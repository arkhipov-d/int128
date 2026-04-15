#include "Int128.hpp"
#include <algorithm>

Int128::Int128(int64_t value) {
    high_ = (value < 0) ? UINT64_MAX : 0;
    low_ = static_cast<uint64_t>(value);
}

bool Int128::is_negative() const {
    return (high_ >> 63) != 0;
}

Int128 Int128::operator-() const {
    Int128 res(~high_, ~low_);
    res.low_ += 1;
    if (res.low_ == 0) {
        res.high_ += 1;
    }
    return res;
}

Int128& Int128::operator+=(const Int128& o) {
    uint64_t old_low = low_;
    low_ += o.low_;
    high_ += o.high_;
    if (low_ < old_low) {
        high_ += 1;
    }
    return *this;
}

Int128& Int128::operator-=(const Int128& o) {
    uint64_t old_low = low_;
    low_ -= o.low_;
    high_ -= o.high_;
    if (low_ > old_low) {
        high_ -= 1;
    }
    return *this;
}

Int128 Int128::mul_unsigned(const Int128& a, const Int128& b) {
    Int128 res;
    Int128 cur = a;

    for (int i = 0; i < 128; ++i) {
        bool bit = (i < 64) ? ((b.low_ >> i) & 1)
            : ((b.high_ >> (i - 64)) & 1);

        if (bit) res += cur;

        bool carry = (cur.low_ >> 63) & 1;
        cur.low_ <<= 1;
        cur.high_ <<= 1;
        if (carry) cur.high_ |= 1;
    }

    return res;
}

bool Int128::less_unsigned(const Int128& a, const Int128& b) {
    if (a.high_ != b.high_) return a.high_ < b.high_;
    return a.low_ < b.low_;
}

Int128 Int128::div_unsigned(Int128 a, Int128 b) {
    Int128 res;
    Int128 cur;

    for (int i = 127; i >= 0; --i) {
        bool carry = (cur.low_ >> 63) & 1;
        cur.low_ <<= 1;
        cur.high_ <<= 1;
        if (carry) cur.high_ |= 1;

        if (i < 64) {
            if ((a.low_ >> i) & 1) cur.low_ |= 1;
        }
        else {
            if ((a.high_ >> (i - 64)) & 1) cur.low_ |= 1;
        }

        if (!less_unsigned(cur, b)) {
            cur -= b;

            if (i < 64) res.low_ |= (1ULL << i);
            else res.high_ |= (1ULL << (i - 64));
        }
    }

    return res;
}

Int128& Int128::operator*=(const Int128& o) {
    bool neg = is_negative() ^ o.is_negative();
    Int128 res = mul_unsigned(this->abs(), o.abs());
    *this = neg ? -res : res;
    return *this;
}

Int128& Int128::operator/=(const Int128& o) {
    bool neg = is_negative() ^ o.is_negative();
    Int128 res = div_unsigned(this->abs(), o.abs());
    *this = neg ? -res : res;
    return *this;
}

bool Int128::operator==(const Int128& o) const {
    return high_ == o.high_ && low_ == o.low_;
}

bool Int128::operator!=(const Int128& o) const {
    return !(*this == o);
}

bool Int128::operator<(const Int128& o) const {
    if (is_negative() != o.is_negative()) return is_negative();
    if (high_ != o.high_) return high_ < o.high_;
    return low_ < o.low_;
}

bool Int128::operator>(const Int128& o) const {
    return o < *this;
}

bool Int128::operator<=(const Int128& o) const {
    return !(*this > o);
}

bool Int128::operator>=(const Int128& o) const {
    return !(*this < o);
}

Int128 Int128::abs() const {
    return is_negative() ? -(*this) : *this;
}

std::string Int128::str() const {
    if (*this == Int128(0)) return "0";

    Int128 tmp = abs();
    std::string s;

    while (tmp != Int128(0)) {
        Int128 div = div_unsigned(tmp, Int128(10));
        Int128 mul = div;
        mul *= Int128(10);

        Int128 rem = tmp;
        rem -= mul;

        s.push_back('0' + static_cast<int>(rem.low_));
        tmp = div;
    }

    if (is_negative()) s.push_back('-');
    std::reverse(s.begin(), s.end());
    return s;
}

Int128::Int128(std::string_view sv) : high_(0), low_(0) {
    bool neg = false;
    size_t i = 0;

    if (!sv.empty() && sv[0] == '-') {
        neg = true;
        i = 1;
    }

    for (; i < sv.size(); ++i) {
        *this *= Int128(10);
        *this += Int128(static_cast<int64_t>(sv[i] - '0'));
    }

    if (neg) *this = -*this;
}

Int128::operator int64_t() const {
    return static_cast<int64_t>(low_);
}

Int128::operator double() const {
    double res = 0;
    double base = 1;
    Int128 tmp = abs();

    for (int i = 0; i < 128; ++i) {
        bool bit = (i < 64) ? ((tmp.low_ >> i) & 1)
            : ((tmp.high_ >> (i - 64)) & 1);
        if (bit) res += base;
        base *= 2.0;
    }

    return is_negative() ? -res : res;
}

std::ostream& operator<<(std::ostream& os, const Int128& v) {
    return os << v.str();
}