#ifndef HW4_HUFFMAN_DEC_H
#define HW4_HUFFMAN_DEC_H

#include <unordered_map>
#include "power_bitset.h"
#include "huffman_tree.h"
#include "symbol_counter.h"


struct huffman_dec {
    huffman_dec(std::vector<uint8_t> const &tree_info, size_t tree_size, size_t alph_size);

    std::vector<uint8_t> decode_block(power_bitset<uint64_t> const &bits);

private:
    huffman_tree tree;
    std::vector<uint8_t> data;
};

#endif //HW4_HUFFMAN_DEC_H
