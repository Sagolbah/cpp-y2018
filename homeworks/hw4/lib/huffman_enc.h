#ifndef HW4_HUFFMAN_ENC_H
#define HW4_HUFFMAN_ENC_H

#include <unordered_map>
#include "power_bitset.h"
#include "huffman_tree.h"
#include "symbol_counter.h"


struct huffman_enc {
    huffman_enc(symbol_counter const &symbols);

    power_bitset<uint64_t> encode_block(size_t sz, const uint8_t *src);

    void encode_tree_data(power_bitset<uint64_t> &bs, std::vector<uint8_t> &alph);

private:
    std::unordered_map<uint8_t, power_bitset<uint64_t>> symbol_dict;
    huffman_tree tree;
};

#endif //HW4_HUFFMAN_ENC_H
