#ifndef HW4_SYMBOL_COUNTER_H
#define HW4_SYMBOL_COUNTER_H

#include <vector>
#include <cstdint>

struct symbol_counter {
    symbol_counter();

    void add_frequency(size_t sz, const uint8_t *data);

    std::vector<uint64_t> getFreqs() const;

private:
    std::vector<uint64_t> freqs;
};

#endif
