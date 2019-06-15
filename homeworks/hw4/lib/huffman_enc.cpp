#include "huffman_enc.h"

power_bitset<uint64_t> huffman_enc::encode_block(size_t sz, const uint8_t *src) {
    power_bitset<uint64_t> result;
    for (size_t i = 0; i < sz; i++) {
        result.merge(symbol_dict[src[i]]);
    }
    return result;
}

huffman_enc::huffman_enc(symbol_counter const &freqs) : tree(freqs.getFreqs()) {
    symbol_dict = tree.encode_alpha();
}

void huffman_enc::encode_tree_data(power_bitset<uint64_t> &bs, std::vector<uint8_t> &alph) {
    tree.encode_data(bs, alph);
}

