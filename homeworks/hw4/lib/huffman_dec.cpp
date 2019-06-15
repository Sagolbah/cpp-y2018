#include "huffman_dec.h"

huffman_dec::huffman_dec(std::vector<uint8_t> const &tree_info, size_t tree_size, size_t alph_size) :
        tree(tree_info, tree_size, alph_size) {}

std::vector<uint8_t> huffman_dec::decode_block(power_bitset<uint64_t> const &bits) {
    std::vector<uint8_t> res;
    size_t ind = 0;
    uint8_t symbol = '\0';
    while (tree.get_symbol(bits, ind, symbol)){
        res.push_back(symbol);
    }
    return res;
}

