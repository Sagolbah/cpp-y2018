#include <cassert>
#include <algorithm>
#include <stdexcept>
#include "huffman_tree.h"

huffman_tree::huffman_tree(const std::vector<uint64_t> &freqs) {
    assert(freqs.size() == ALPHABET_SIZE);
    const uint64_t INF = 1ll << 59;
    head = root;
    std::vector<std::pair<uint64_t, node *>> nodes;
    for (size_t i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            nodes.push_back({freqs[i], new node(uint8_t(i))});
        }
    }
    if (nodes.empty()) {
        throw std::runtime_error("No frequencies detected.");
    }
    std::sort(nodes.begin(), nodes.end());
    bool flag_empty_child = nodes.size() == 1;
    // Huffman $\mathcal{O}(n)$ encoding
    std::vector<std::pair<uint64_t, node *>> freqsum;
    size_t prev_size = nodes.size();
    nodes.emplace_back(INF, nullptr);
    nodes.emplace_back(INF, nullptr);
    size_t freq_ptr = 0, fsum_ptr = 0;
    while (fsum_ptr < (freqsum.size() - 1) || freq_ptr < prev_size) {
        uint64_t fr1 = nodes[freq_ptr].first, fr2 = nodes[freq_ptr + 1].first;
        uint64_t fs1 = (fsum_ptr < freqsum.size()) ? freqsum[fsum_ptr].first : INF;
        uint64_t fs2 = (fsum_ptr + 1 < freqsum.size()) ? freqsum[fsum_ptr + 1].first : INF;
        uint64_t res;
        node *cur;
        if (fr1 + fr2 <= fs1 + fs2 && fr1 + fr2 <= fr1 + fs1) {
            cur = new node(nodes[freq_ptr].second, nodes[freq_ptr + 1].second);
            freq_ptr += 2;
            res = fr1 + fr2;
        } else if (fs1 + fs2 <= fr1 + fr2 && fs1 + fs2 <= fs1 + fr1) {
            cur = new node(freqsum[fsum_ptr].second, freqsum[fsum_ptr + 1].second);
            fsum_ptr += 2;
            res = fs1 + fs2;
        } else {
            cur = new node(nodes[freq_ptr].second, freqsum[fsum_ptr].second);
            ++freq_ptr;
            ++fsum_ptr;
            res = fs1 + fr1;
        }
        freqsum.push_back({res, cur});
    }
    root = freqsum[freqsum.size() - 1].second;
    if (flag_empty_child) {
        root->right = new node(0);
    }
}

huffman_tree::huffman_tree(std::vector<uint8_t> const &tree_info, size_t tree_size, size_t alph_size) {
    power_bitset<uint64_t> bitset(tree_size, tree_info.data());
    std::vector<uint8_t> alphabet(alph_size);
    size_t alpha_index = (tree_size / (sizeof(uint64_t) * 8) +
                          ((tree_size % (sizeof(uint64_t) * 8)) != 0)) * sizeof(uint64_t);
    for (size_t i = 0; i < alph_size; i++) {
        alphabet[i] = tree_info[i + alpha_index];
    }
    root = regenerate_tree(bitset, alphabet);
    head = root;
}

huffman_tree::node *huffman_tree::regenerate_tree(power_bitset<uint64_t> const &bs, std::vector<uint8_t> const &alph) {
    if (bs.size() <= regen_ind_tree) {
        throw std::runtime_error("File is damaged - probably missing info for decoding.");
    }
    bool cur_bit = bs.get_bit(regen_ind_tree);
    regen_ind_tree++;
    if (cur_bit) {
        if (alph.size() <= regen_ind_alph) {
            throw std::runtime_error("File is damaged - probably missing info for decoding.");
        }
        return new node(alph[regen_ind_alph++]);
    }
    node *res = new node(0);
    res->left = regenerate_tree(bs, alph);
    res->right = regenerate_tree(bs, alph);
    return res;
}

huffman_tree::~huffman_tree() {
    delete root;
}

std::unordered_map<uint8_t, power_bitset<uint64_t>> huffman_tree::encode_alpha() {
    std::unordered_map<uint8_t, power_bitset<uint64_t>> res;
    power_bitset<uint64_t> bitseq;
    encode_dfs_symbols(res, root, bitseq);
    return res;
}

void huffman_tree::encode_dfs_symbols(std::unordered_map<uint8_t, power_bitset<uint64_t>> &mp, huffman_tree::node *cur,
                                      power_bitset<uint64_t> &seq) {
    if (cur->left) {
        seq.push_bit(0);
        encode_dfs_symbols(mp, cur->left, seq);
        seq.remove_last_bit();
        seq.push_bit(1);
        encode_dfs_symbols(mp, cur->right, seq);
        seq.remove_last_bit();
    } else {
        // Leaf here (invariant)
        mp[cur->chr] = seq;
    }
}

void huffman_tree::encode_data(power_bitset<uint64_t> &bs, std::vector<uint8_t> &alph) {
    encode_tree_path(root, root, bs, alph);
}

void huffman_tree::encode_tree_path(huffman_tree::node *cur, huffman_tree::node *par, power_bitset<uint64_t> &bitset,
                                    std::vector<uint8_t> &alph) {
    if (cur->left) {
        // down left
        bitset.push_bit(0);
        encode_tree_path(cur->left, cur, bitset, alph);
        encode_tree_path(cur->right, cur, bitset, alph);
    } else {
        alph.push_back(cur->chr);
    }
    if (cur != par->right) {
        bitset.push_bit(1);
    }
}

bool huffman_tree::get_symbol(power_bitset<uint64_t> const &bs, size_t &index, uint8_t &res) {
    // returns true if parsed symbol, false otherwise
    head = symbol_tree_path(head, index, bs, res);
    if (!head) {
        head = root;
        return true;
    }
    return false;
}

huffman_tree::node *
huffman_tree::symbol_tree_path(huffman_tree::node *cur, size_t &ind, power_bitset<uint64_t> const &bs, uint8_t &symb) {
    if (!cur->left) {
        // leaf
        symb = cur->chr;
        return nullptr;
    }
    if (ind == bs.size()) {
        // end of block
        return cur;
    }
    if (bs.get_bit(ind) == 0) {
        return symbol_tree_path(cur->left, ++ind, bs, symb);
    }
    return symbol_tree_path(cur->right, ++ind, bs, symb);
}



