#ifndef HW4_HUFFMAN_TREE_H
#define HW4_HUFFMAN_TREE_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "power_bitset.h"

struct huffman_tree {
    // Node
    struct node {
        uint8_t chr;
        node *left;
        node *right;

        node(node *l, node *r) {
            chr = 0;  // not leaf
            left = l;
            right = r;
        }

        node(uint8_t chr) : chr(chr), left(nullptr), right(nullptr) {}

        ~node() {
            delete left;
            delete right;
        }
    };

    node *root = nullptr;
    node *head = nullptr;

    // Constructors
    huffman_tree(std::vector<uint64_t> const &freqs);

    huffman_tree(std::vector<uint8_t> const &tree_info, size_t tree_size, size_t alph_size);

    std::unordered_map<uint8_t, power_bitset<uint64_t>> encode_alpha();

    ~huffman_tree();

    void encode_data(power_bitset<uint64_t> &bs, std::vector<uint8_t> &alph);

    bool get_symbol(power_bitset<uint64_t> const &bs, size_t &index, uint8_t &res);

    node *symbol_tree_path(node *cur, size_t &ind, power_bitset<uint64_t> const &bs, uint8_t &symb);

private:
    static const uint64_t ALPHABET_SIZE = 256;

    void encode_dfs_symbols(std::unordered_map<uint8_t, power_bitset<uint64_t>> &mp, node *cur,
                            power_bitset<uint64_t> &seq);

    void encode_tree_path(node *cur, node *par, power_bitset<uint64_t> &bitset, std::vector<uint8_t> &alph);

    node *regenerate_tree(power_bitset<uint64_t> const &bs, std::vector<uint8_t> const &alph);

    size_t regen_ind_tree = 0, regen_ind_alph = 0;

};

#endif //HW4_HUFFMAN_TREE_H
