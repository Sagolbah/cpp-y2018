#include "big_integer.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

__extension__ typedef unsigned __int128 uint128_t;

big_integer::big_integer() {
    value.push_back(0);
    sign = 1;
}

big_integer::big_integer(big_integer const &other) {
    value = other.value;
    sign = other.sign;
}

big_integer::big_integer(int a) {
    if (a == INT32_MIN) {
        value.push_back(2147483648u);
    } else {
        value.push_back(static_cast<uint32_t>(abs(a)));
    }
    sign = (a >= 0) ? 1 : -1;
}

big_integer::big_integer(std::string const &str) {
    value.push_back(0);
    big_integer base = 1;
    for (int32_t i = static_cast<int32_t>(str.length()) - 1;
         i >= 0 && str[i] != '-'; i--) {
        auto addition = static_cast<uint32_t>(str[i] - '0');
        *this += base * addition;
        base *= 10;
    }
    sign = str[0] == '-' ? -1 : 1;
    shrink_to_fit();
}

big_integer::~big_integer() {
    // value.clear();
}

void big_integer::shrink_to_fit() {
    while (value.size() > 1 && value[value.size() - 1] == 0) {
        value.pop_back();
    }
}

big_integer big_integer::get_adding_code() const {
    if (sign > 0) {
        return *this;
    }
    big_integer res;
    res.value.pop_back();
    for (const auto i : value) {
        res.value.push_back(~i);
    }
    res.value.push_back(UINT32_MAX);  // all 1 in additional code
    res += 1;
    res.sign = is_zero() ? 1 : -1;
    return res;
}

big_integer big_integer::complement_to_unsigned() const {
    if (sign > 0) {
        return *this;
    }
    big_integer res;
    res.value.pop_back();
    for (auto i : value) {
        res.value.push_back(~i);
    }
    res += 1;
    res.shrink_to_fit();
    res.sign = sign;
    return res;
}

bool big_integer::is_zero() const {
    return (value.size() == 1 && value[0] == 0);
}

big_integer &big_integer::operator=(big_integer const &other) {
    value = other.value;
    sign = other.sign;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    return (*this = *this + rhs);
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return (*this = *this - rhs);
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    return (*this = *this * rhs);
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    return (*this = *this / rhs);
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return (*this = *this % rhs);
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return (*this = *this & rhs);
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return (*this = *this | rhs);
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return (*this = *this ^ rhs);
}

big_integer &big_integer::operator<<=(int rhs) {
    return (*this = *this << rhs);
}

big_integer &big_integer::operator>>=(int rhs) {
    return (*this = *this >> rhs);
}

big_integer big_integer::operator+() const { return *this; }

big_integer big_integer::operator-() const {
    big_integer res(*this);
    if (!res.is_zero()) {
        res.sign *= -1;
    } else {
        res.sign = 1;
    }
    return res;
}

big_integer big_integer::operator~() const {
    big_integer res(*this);
    // ~X = -(~(~X) + 1) = -(X + 1)
    return -(res + 1);
}

big_integer &big_integer::operator++() { return (*this = *this + 1); }

big_integer big_integer::operator++(int) {
    big_integer res = *this;
    *this = *this + 1;
    return res;
}

big_integer &big_integer::operator--() { return (*this = *this - 1); }

big_integer big_integer::operator--(int) {
    big_integer res = *this;
    *this = *this - 1;
    return res;
}

big_integer operator+(big_integer const &a, big_integer const &b) {
    if (b.sign < 0) {
        return a - (-b);
    } else if (a.sign < 0) {
        return b - (-a);
    }
    big_integer res = a;
    uint64_t carry = 0;
    for (size_t i = 0; i < std::max(res.value.size(), b.value.size()) || carry; i++) {
        if (i == res.value.size()) {
            res.value.push_back(0);
        }
        uint64_t add_res = res.value[i] + carry + (i < b.value.size() ? b.value[i] : 0);
        carry = (add_res >= (uint64_t(UINT32_MAX) + 1) ? 1 : 0);
        if (carry) {
            add_res -= (uint64_t(UINT32_MAX) + 1);
        }
        res.value[i] = static_cast<uint32_t>(add_res);
    }
    return res;
}

big_integer operator-(big_integer const &a, big_integer const &b) {
    if (a.sign < 0) {
        return -((-a) + b);
    } else if (b.sign < 0) {
        return a + (-b);
    }
    int32_t result_sign = (a > b) ? 1 : -1;
    auto greater = a, lesser = b;
    if (result_sign == -1) {
        std::swap(greater, lesser);
    }
    int64_t carry = 0;
    for (size_t i = 0; i < lesser.value.size() || carry; i++) {
        int64_t sub_res = greater.value[i] - (carry + (i < lesser.value.size() ? lesser.value[i] : 0));
        carry = (sub_res < 0) ? 1 : 0;
        if (carry) {
            sub_res += (uint64_t(UINT32_MAX) + 1);
        }
        greater.value[i] = static_cast<uint32_t>(sub_res);
    }
    greater.shrink_to_fit();
    greater.sign = greater.is_zero() ? 1 : result_sign;
    return greater;
}

std::pair<big_integer, uint32_t> big_integer::div_long_short(uint32_t x) const {
    if (x == 0) {
        throw std::runtime_error("Division by zero");
    }
    uint64_t carry = 0;
    big_integer res;
    res.value.pop_back();
    res.value.resize(value.size());
    for (int32_t i = static_cast<int32_t>(value.size()) - 1; i >= 0; i--) {
        uint64_t cur_val = value[i] + carry * (uint64_t(UINT32_MAX) + 1);
        res.value[i] = static_cast<uint32_t>(cur_val / x);
        carry = cur_val % x;
    }
    res.sign = res.is_zero() ? 1 : sign;
    res.shrink_to_fit();
    return {res, carry};
}

big_integer operator*(big_integer const &a, big_integer const &b) {
    big_integer res;
    res.value.resize(a.value.size() + b.value.size());
    uint64_t carry = 0;
    res.sign = a.sign * b.sign;
    for (uint32_t i = 0; i < a.value.size(); i++) {
        for (uint32_t j = 0; j < b.value.size() || carry; j++) {
            uint128_t mul_res = res.value[i + j] + a.value[i] * 1ull * (j < b.value.size() ? b.value[j] : 0) + carry;
            res.value[i + j] = static_cast<uint32_t>(mul_res % (uint64_t(UINT32_MAX) + 1));
            carry = static_cast<uint32_t>(mul_res / (uint64_t(UINT32_MAX) + 1));
        }
    }
    res.shrink_to_fit();
    if (res.is_zero()) {
        res.sign = 1;
    }
    return res;
}

bool big_integer::smaller(big_integer const &dq, uint64_t k, uint64_t m) const {
    uint64_t i = m, j = 0;
    while (i != j) {
        if (value[i + k] != dq.value[i]) {
            j = i;
        } else {
            i--;
        }
    }
    return value[i + k] < dq.value[i];
}

uint32_t big_integer::trial(uint64_t k, uint64_t m, uint64_t const d2) const {
    const uint128_t BASE = uint128_t(UINT32_MAX) + 1;
    uint64_t km = k + m;
    uint128_t r3 = (uint128_t(value[km]) * BASE + uint128_t(value[km - 1])) * BASE + uint128_t(value[km - 2]);
    return uint32_t(std::min(r3 / uint128_t(d2), uint128_t(UINT32_MAX)));
}

void big_integer::difference(big_integer const &dq, uint64_t k, uint64_t m) {
    int64_t borrow = 0, diff;
    const int64_t BASE = int64_t(UINT32_MAX) + 1;
    for (uint64_t i = 0; i < m + 1; i++) {
        diff = int64_t(value[i + k]) - int64_t(dq.value[i]) - borrow + BASE;
        value[i + k] = uint32_t(diff % BASE);
        borrow = 1 - diff / BASE;
    }
}

big_integer operator/(big_integer const &a, big_integer const &b) {
    if (b.is_zero()) {
        throw std::runtime_error("Division by zero");
    } else if (a.value.size() < b.value.size()) {
        big_integer res;
        return res;
    } else if (b.value.size() == 1) {
        return b.sign > 0 ? a.div_long_short(b.value[0]).first : -(a.div_long_short(b.value[0]).first);
    }
    int32_t res_sign = a.sign * b.sign;
    auto n = a.value.size(), m = b.value.size();
    auto f = (1L << 32) / (uint64_t(b.value[m - 1]) + 1);
    big_integer r = a * f;
    big_integer d = b * f;
    r.sign = 1;
    d.sign = 1;
    big_integer q;
    q.value.resize(n - m + 1);
    r.value.push_back(0);
    const uint64_t d2 = (uint64_t(d.value[m - 1]) << 32) + uint64_t(d.value[m - 2]);
    for (auto k = int32_t(n - m); k >= 0; k--) {
        // precondition: t has k + m + 1 digits
        auto qt = r.trial(uint64_t(k), m, d2);
        big_integer qt_mul;
        qt_mul.value[0] = qt;
        big_integer dq = qt_mul * d;
        dq.value.resize(m + 1);
        if (r.smaller(dq, uint64_t(k), m)) {
            qt--;
            dq = d * qt;
        }
        q.value[k] = qt;
        r.difference(dq, uint64_t(k), m);
    }
    q.sign = res_sign;
    q.shrink_to_fit();
    return q;
}

big_integer operator%(big_integer const &a, big_integer const &b) {
    return a - (a / b) * b;
}

big_integer big_integer::logical_op(big_integer const &a, big_integer const &b, uint32_t (*op)(uint32_t, uint32_t),
                                    int32_t (*sign)(int32_t, int32_t)) const {
    big_integer greater = a.get_adding_code();
    big_integer lesser = b.get_adding_code();
    if (greater.value.size() < lesser.value.size()) {
        std::swap(greater, lesser);
    }
    for (size_t i = 0; i < lesser.value.size(); i++) {
        greater.value[i] = (*op)(greater.value[i], lesser.value[i]);
    }
    for (size_t i = lesser.value.size(); i < greater.value.size(); i++) {
        greater.value[i] = (*op)(greater.value[i], (lesser.sign < 0 ? UINT32_MAX : 0));
    }
    greater.sign = (*sign)(a.sign, b.sign);
    greater.shrink_to_fit();
    return greater.complement_to_unsigned();
}

big_integer operator&(big_integer const &a, big_integer const &b) {
    return a.logical_op(a, b, [](uint32_t x, uint32_t y) { return x & y; },
            [](int32_t x, int32_t y) { return (x < 0 && y < 0) ? -1 : 1; });
}

big_integer operator|(big_integer const &a, big_integer const &b) {
    return a.logical_op(a, b, [](uint32_t x, uint32_t y) { return x | y; },
            [](int32_t x, int32_t y) { return (x < 0 || y < 0) ? -1 : 1; });
}

big_integer operator^(big_integer const &a, big_integer const &b) {
    return a.logical_op(a, b, [](uint32_t x, uint32_t y) { return x ^ y; },
                        [](int32_t x, int32_t y) { return (x < 0 && y > 0) || (x > 0 && y < 0) ? -1 : 1; });
}

const uint32_t BITS_IN_DIGIT = 32;

big_integer operator<<(big_integer const &a, int b) {
    if (b < 0) {
        return a >> abs(b);
    } else if (b == 0) {
        return big_integer(a);
    }
    auto digit_shift = static_cast<uint32_t>(b);
    auto bitshift = digit_shift % BITS_IN_DIGIT;
    digit_shift /= BITS_IN_DIGIT;
    big_integer res;
    res.value.pop_back();
    for (uint32_t i = 0; i < digit_shift; i++) {
        res.value.push_back(0);
    }
    for (auto i : a.value) {
        res.value.push_back(i);
    }
    uint32_t remainder = 0;
    for (size_t i = digit_shift; i < res.value.size(); i++) {
        uint32_t tmp = (res.value[i] << bitshift);
        tmp += remainder;
        remainder = (res.value[i] >> (BITS_IN_DIGIT - bitshift));
        res.value[i] = tmp;
    }
    res.value.push_back(remainder);
    res.shrink_to_fit();
    res.sign = res.is_zero() ? 1 : a.sign;
    return res;
}

big_integer operator>>(big_integer const &a, int b) {
    if (b < 0) {
        return a << abs(b);
    } else if (b == 0) {
        return big_integer(a);
    }
    big_integer num = a;
    auto digit_shift = static_cast<uint32_t>(b);
    auto bitshift = digit_shift % BITS_IN_DIGIT;
    digit_shift /= BITS_IN_DIGIT;
    if (num.sign < 0) {
        num = num.get_adding_code();
        // adding leading infinite 111... sequence for negative number to emulate two's complement.
        // first 111... uint32_t already got in get_adding_code();
        for (int32_t i = 0; i < int32_t(digit_shift) - 1; i++) {
            num.value.push_back(UINT32_MAX);
        }
    }
    big_integer res;
    if (digit_shift >= num.value.size()) {
        return res;
    }
    res.value.pop_back();
    for (uint32_t i = digit_shift; i < num.value.size(); i++) {
        res.value.push_back(num.value[i]);
    }
    for (uint32_t i = 0; i < res.value.size() - 1; i++) {
        res.value[i] >>= bitshift;
        // getting last bits
        uint32_t moving_part = res.value[i + 1] << (BITS_IN_DIGIT - bitshift);
        res.value[i] += moving_part;
    }
    res.value[res.value.size() - 1] >>= bitshift;
    res.shrink_to_fit();
    res.sign = num.sign;
    if (res.sign < 0) {
        res.value.pop_back();  // remove leading infinite 111... sequence for negative number.
    }
    res = res.complement_to_unsigned();
    res.sign = res.is_zero() ? 1 : res.sign;
    return res;
}

bool operator==(big_integer const &a, big_integer const &b) {
    if (a.is_zero() && b.is_zero()) {
        return true;
    }
    if (a.value.size() != b.value.size() || a.sign != b.sign) {
        return false;
    }
    for (uint32_t i = 0; i < a.value.size(); i++) {
        if (a.value[i] != b.value[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign < b.sign;
    } else if (a.value.size() < b.value.size()) {
        return a.sign == 1;
    } else if (a.value.size() > b.value.size()) {
        return a.sign == -1;
    } else if (a.sign == -1) {
        return (-a) > (-b);
    }
    for (int32_t i = static_cast<int32_t>(a.value.size()) - 1; i >= 0; i--) {
        if (a.value[i] < b.value[i]) {
            return true;
        } else if (a.value[i] > b.value[i]) {
            return false;
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign > b.sign;
    } else if (a.value.size() < b.value.size()) {
        return a.sign == -1;
    } else if (a.value.size() > b.value.size()) {
        return a.sign == 1;
    } else if (a.sign == -1) {
        return (-a) < (-b);
    }
    for (int32_t i = static_cast<int32_t>(a.value.size()) - 1; i >= 0; i--) {
        if (a.value[i] > b.value[i]) {
            return true;
        } else if (a.value[i] < b.value[i]) {
            return false;
        }
    }
    return false;
}

bool operator<=(big_integer const &a, big_integer const &b) { return !(a > b); }

bool operator>=(big_integer const &a, big_integer const &b) { return !(a < b); }

std::string to_string(big_integer const &a) {
    std::string res;
    big_integer cur = a;
    bool is_negative = cur.sign == -1;
    if (cur.is_zero()) {
        return "0";
    }
    while (!cur.is_zero()) {
        auto div_res = cur.div_long_short(10);
        cur = div_res.first;
        res.push_back('0' + char(div_res.second));
    }
    if (is_negative) {
        res.push_back('-');
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    s << to_string(a);
    return s;
}
