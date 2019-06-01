#include <cstring>
#include "uintvector.h"

uintvector::bigvector::bigvector() {
    capacity = 0;
    data = nullptr;
}

uintvector::bigvector::bigvector(size_t cap) : capacity(cap),
                                               data(new uint32_t[cap], std::default_delete<uint32_t[]>()) {
    memset(data.get(), 0, cap * sizeof(uint32_t));
}


void uintvector::bigvector::copy(const uintvector::bigvector &other) {
    capacity = other.capacity;
    // data = std::shared_ptr<uint32_t>(other.data);
    data = other.data;
}

uintvector::bigvector::~bigvector() = default;


uintvector::uintvector() : bigvect() {
    _size = 0;
    vec_data = smallvect;
    is_big = false;
}


uintvector::~uintvector() {
    if (is_big) {
        bigvect.~bigvector();
    }
}

uintvector::uintvector(uintvector const &other) {
    _size = other._size;
    if (other.is_big) {
        bigvect.copy(other.bigvect);
        vec_data = bigvect.data.get();
    } else {
        vec_data = smallvect;
        memcpy(vec_data, other.smallvect, SMALL_SIZE * sizeof(uint32_t));
    }
}

size_t uintvector::get_capacity() {
    return is_big ? bigvect.capacity : SMALL_SIZE;
}

void uintvector::unique_copy() {
    // called after each modification, if not unique - creates unique copy
    if (is_big && !bigvect.data.unique()) {
        bigvector new_vect(bigvect.capacity);
        memcpy(new_vect.data.get(), bigvect.data.get(), bigvect.capacity * sizeof(uint32_t));
        bigvect = new_vect;
        vec_data = bigvect.data.get();
    }
}

uintvector &uintvector::operator=(uintvector const &other) {
    _size = other._size;
    if (other.is_big) {
        if (!is_big) {
            memset(smallvect, 0, sizeof(uint32_t) * SMALL_SIZE);
        }
        bigvect.copy(other.bigvect);
        vec_data = bigvect.data.get();
    } else {
        if (is_big) {
            bigvect.data = nullptr;  // triggers shared_ptr
        }
        vec_data = smallvect;
        memcpy(smallvect, other.smallvect, SMALL_SIZE * sizeof(uint32_t));
    }
    is_big = other.is_big;
    return *this;
}

void uintvector::resize(size_t sz) {
    unique_copy();
    // bool flag_was_small = get_capacity() == SMALL_SIZE;
    _size = sz;
    if (_size <= get_capacity()) {
        return;
    }
    bigvector new_vect(sz);
    memcpy(new_vect.data.get(), vec_data, sizeof(uint32_t) * get_capacity());
    if (!is_big) {
        is_big = true;
        memset(smallvect, 0, sizeof(uint32_t) * SMALL_SIZE);
    }
    bigvect = new_vect;
    vec_data = bigvect.data.get();
}


uint32_t &uintvector::operator[](size_t index) const {
    return vec_data[index];
}

void uintvector::pop_back() {
    unique_copy();
    _size--;
}

void uintvector::push_back(uint32_t x) {
    // Create unique copy if necessary
    unique_copy();
    // Allocation:
    size_t old_cap = get_capacity();
    bool flag_was_small = old_cap == SMALL_SIZE;
    _size++;
    if (_size > old_cap) {
        // allocation needed
        bigvector new_vect(2 * old_cap);
        memcpy(new_vect.data.get(), vec_data, sizeof(uint32_t) * old_cap);
        if (flag_was_small) {
            is_big = true;
            memset(smallvect, 0, sizeof(uint32_t) * SMALL_SIZE);
        }
        bigvect = new_vect;
        vec_data = bigvect.data.get();
    }
    // put element
    vec_data[_size - 1] = x;
}

size_t uintvector::size() const {
    return _size;
}

uintvector &uintvector::modify(size_t index, uint32_t val) {
    unique_copy();
    vec_data[index] = val;
    return *this;
}


