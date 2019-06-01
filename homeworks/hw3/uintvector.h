#ifndef BIGINT_HW3_UINTVECTOR_H
#define BIGINT_HW3_UINTVECTOR_H

#include <cstdint>
#include <cstdlib>
#include <memory>

struct uintvector {
private:
    struct bigvector {
        size_t capacity;
        std::shared_ptr<uint32_t> data;

        void copy(bigvector const &other);

        bigvector();

        ~bigvector();

        explicit bigvector(size_t cap);

    };

    size_t _size;
    static const size_t SMALL_SIZE = sizeof(bigvector) / sizeof(uint32_t);
    bool is_big;
    union {
        bigvector bigvect;
        uint32_t smallvect[SMALL_SIZE];
    };
    uint32_t *vec_data;

    size_t get_capacity();

    void unique_copy();

public:
    uintvector();

    uintvector(uintvector const &other);

    ~uintvector();

    uintvector &operator=(uintvector const &other);

    void push_back(uint32_t x);

    void pop_back();

    void resize(size_t sz);

    uintvector &modify(size_t index, uint32_t val);

    uint32_t &operator[](size_t index) const;

    size_t size() const;

};

#endif //BIGINT_HW3_UINTVECTOR_H
