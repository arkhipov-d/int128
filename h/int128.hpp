#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <cstdint>

class Int128 {
public:
    Int128() : high_(0), low_(0) {}
    Int128(int64_t value);
    explicit Int128(std::string_view str);

    explicit operator int64_t() const;
    explicit operator double() const;

    std::string str() const;

    Int128 operator-() const;

    Int128& operator+=(const Int128& other);
    Int128& operator-=(const Int128& other);
    Int128& operator*=(const Int128& other);
    Int128& operator/=(const Int128& other);

    bool operator==(const Int128& other) const;
    bool operator!=(const Int128& other) const;
    bool operator<(const Int128& other) const;
    bool operator>(const Int128& other) const;
    bool operator<=(const Int128& other) const;
    bool operator>=(const Int128& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Int128& value);

private:
    uint64_t high_;
    uint64_t low_;

    Int128(uint64_t high, uint64_t low) : high_(high), low_(low) {}

    bool is_negative() const;
    Int128 abs() const;

    static Int128 mul_unsigned(const Int128& a, const Int128& b);
    static Int128 div_unsigned(Int128 a, Int128 b);
    static bool less_unsigned(const Int128& a, const Int128& b);
};

inline Int128 operator+(Int128 lhs, const Int128& rhs) {
    lhs += rhs;
    return lhs;
}

inline Int128 operator-(Int128 lhs, const Int128& rhs) {
    lhs -= rhs;
    return lhs;
}

inline Int128 operator*(Int128 lhs, const Int128& rhs) {
    lhs *= rhs;
    return lhs;
}

inline Int128 operator/(Int128 lhs, const Int128& rhs) {
    lhs /= rhs;
    return lhs;
}