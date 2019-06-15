#include "symbol_counter.h"

symbol_counter::symbol_counter() : freqs(256) {}

void symbol_counter::add_frequency(size_t sz, const uint8_t *data) {
    for (size_t i = 0; i < sz; i++) {
        freqs[data[i]]++;
    }
}

std::vector<uint64_t> symbol_counter::getFreqs() const {
    return freqs;
}
