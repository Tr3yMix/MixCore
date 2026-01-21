#pragma once

#include <climits>
#include <cstdint>
#include <iostream>

namespace Coreful::math {

    struct Uint32{

    private:
        uint32_t value;

    public:

        constexpr Uint32(): value(0) {}

        constexpr explicit Uint32(const uint32_t value) : value(value) {}
        constexpr explicit Uint32(const int32_t value) : value(value < 0 ? 0 : static_cast<uint32_t>(value)) {}

        [[nodiscard]]constexpr uint32_t raw() const {return value;}

        [[nodiscard]]constexpr int32_t signedInt() const {return static_cast<int32_t>(value);}

        //Conversions
        [[nodiscard]]constexpr float toFloat() const {return static_cast<float>(value);}
        [[nodiscard]]constexpr double toDouble() const {return value;}
        [[nodiscard]]constexpr char toChar() const {return static_cast<char>(value);}
        [[nodiscard]]std::string toString() const {return std::to_string(value);}


        constexpr explicit operator uint32_t() const {return value;}
        constexpr explicit operator int32_t() const {return static_cast<int32_t>(value);}

        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        explicit(false) Uint32(T v) : value(clampToUint32(static_cast<int64_t>(v))) {}


        static constexpr uint32_t clampToUint32(const int64_t value) {
            if (value < 0) return 0;
            if (value > UINT_MAX) return UINT_MAX;
            return static_cast<uint32_t>(value);
        }

        // Comparison
        constexpr bool operator==(const Uint32& other) const {return value == other.value;}
        constexpr bool operator!=(const Uint32& other) const { return value != other.value; }
        constexpr bool operator<(const Uint32& other) const { return value < other.value; }
        constexpr bool operator<=(const Uint32& other) const { return value <= other.value; }
        constexpr bool operator>(const Uint32& other) const { return value > other.value; }
        constexpr bool operator>=(const Uint32& other) const { return value >= other.value; }

        // Arithmetic
        friend constexpr Uint32 operator+(const Uint32& a, const Uint32& b) {
            const uint64_t sum = a.value + b.value;
            return Uint32(sum > UINT_MAX ? UINT_MAX : static_cast<uint32_t>(sum));
        }
        friend constexpr Uint32 operator-(const Uint32& a, const Uint32& b) {
            if (b > a) return Uint32(0); return Uint32(a.value - b.value);
        }
        friend constexpr Uint32 operator*(const Uint32& a, const Uint32& b) {
            const uint64_t product = a.value * b.value;
            return Uint32(product > UINT_MAX ? UINT_MAX : static_cast<uint32_t>(product));
        }
        friend constexpr Uint32 operator/(const Uint32& a, const Uint32& b) {
            if (b.value == 0) throw std::runtime_error("Division by zero");
            return Uint32(a.value / b.value);
        }
        friend constexpr Uint32 operator%(const Uint32& a, const Uint32& b) {
            if (b.value == 0) throw std::runtime_error("Mod by zero");
            return Uint32(a.value % b.value);
        }

        // Compound assignment
        Uint32& operator+=(const Uint32& other) {
            const uint64_t sum = value + other.value;
            value = sum > UINT_MAX ? UINT_MAX : static_cast<uint32_t>(sum);
            return *this;
        }
        Uint32& operator-=(const Uint32& other) {
            value = other.value > value ? 0 : value - other.value;
            return *this;
        }
        Uint32& operator*=(const Uint32& other) {
            const uint64_t product = value * other.value;
            value = product > UINT_MAX ? UINT_MAX : static_cast<uint32_t>(product);
            return *this;
        }
        Uint32& operator/=(const Uint32& other) {
            if (other.value == 0) throw std::runtime_error("Division by zero");
            value /= other.value;
            return *this;
        }
        Uint32& operator%=(const Uint32& other) {
            if (other.value == 0) throw std::runtime_error("Mod by zero");
            value %= other.value;
            return *this;
        }

        // Increment / Decrement
        constexpr Uint32& operator++() {
            if (value != UINT_MAX) ++value;
            return *this;
        }
        constexpr Uint32 operator++(int) {
            const Uint32 tmp = *this;
            if (value != UINT_MAX) ++value;
            return tmp;
        }
        constexpr Uint32& operator--() {
            if (value > 0) --value;
            return *this;
        }
        constexpr Uint32 operator--(int) { const Uint32 tmp = *this;
            if (value > 0) --value;
            return tmp;
        }

        // Bitwise
        friend constexpr Uint32 operator&(const Uint32& a, const Uint32& b) { return Uint32(a.value & b.value); }
        friend constexpr Uint32 operator|(const Uint32& a, const Uint32& b) { return Uint32(a.value | b.value); }
        friend constexpr Uint32 operator^(const Uint32& a, const Uint32& b) { return Uint32(a.value ^ b.value); }
        Uint32& operator&=(const Uint32& other) { value &= other.value; return *this; }
        Uint32& operator|=(const Uint32& other) { value |= other.value; return *this; }
        Uint32& operator^=(const Uint32& other) { value ^= other.value; return *this; }

        // Shift
        friend constexpr Uint32 operator<<(const Uint32& a, const uint32_t shift) { return Uint32(a.value << shift); }
        friend constexpr Uint32 operator>>(const Uint32& a, const uint32_t shift) { return Uint32(a.value >> shift); }
        Uint32& operator<<=(const uint32_t shift) { value <<= shift; return *this; }
        Uint32& operator>>=(const uint32_t shift) { value >>= shift; return *this; }

        // Stream output
        friend std::ostream& operator<<(std::ostream& os, const Uint32& u) {
            return os << u.value;
        }
    };


}



