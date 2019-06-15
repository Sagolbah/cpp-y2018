#ifndef HW4_POWER_BITSET_H
#define HW4_POWER_BITSET_H

// Bitset version with new methods

#include <cstdint>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <cassert>

template<typename T>
struct power_bitset {
    power_bitset() : _size(0) {}

    ~power_bitset() = default;

    power_bitset(std::vector<T> data) : bit(data), _size(data.size() * TYPESIZE) {}

    power_bitset(size_t sz, uint8_t const *data) : bit((sz % TYPESIZE == 0) ? (sz / TYPESIZE) : (sz / TYPESIZE + 1)) {
        _size = sz;
        memcpy(bit.data(), data, bit.size() * sizeof(T));
    }

    size_t size() const {
        return _size;
    }

    void set(size_t ind, bool b) {
        checkRange(ind);
        bit[ind / TYPESIZE] = b ? bit[ind / TYPESIZE] |= (1ll << (ind % TYPESIZE)) :
                bit[ind / TYPESIZE] &= ~(1ll << (ind % TYPESIZE));
    }

    void push_bit(bool b) {
        if ((_size % TYPESIZE) == 0) {
            bit.push_back(0);
        }
        bit[_size / TYPESIZE] = b ? bit[_size / TYPESIZE] |= (1ll << (_size % TYPESIZE)) :
                bit[_size / TYPESIZE] &= ~(1ll << (_size % TYPESIZE));
        ++_size;
    }

    bool get_bit(size_t ind) const {
        checkRange(ind);
        return bool((bit[ind / TYPESIZE] >> (ind % TYPESIZE)) & 1);
    }

    void merge(power_bitset<T> const &other) {
        bit.reserve((_size + other.size()) / TYPESIZE + ((_size + other.size()) % TYPESIZE) != 0);
        for (size_t i = 0; i < other.size(); i++) {
            push_bit(other.get_bit(i));
        }
    }

    void drop_last(size_t cnt) {
        assert(cnt <= _size);
        while (cnt > TYPESIZE) {
            cnt -= TYPESIZE;
            _size -= TYPESIZE;
            bit.pop_back();
        }
        while (cnt > 0) {
            --cnt;
            remove_last_bit();
        }
    }

    void remove_last_bit() {
        _size--;
        if ((_size % TYPESIZE) == 0) {
            bit.pop_back();
        }
    }

    std::vector<T> bs_data() const {
        return bit;
    }

private:
    size_t _size = 0;
    std::vector<T> bit;
    static const size_t TYPESIZE = sizeof(T) * 8;

    void checkRange(size_t a) const {
        if (a >= _size) {
            throw std::runtime_error("bitset index out of range");
        }
    }
};


#endif //HW4_POWER_BITSET_H
