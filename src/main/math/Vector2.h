#pragma once

#include <algorithm>
#include <iostream>
#include <cmath>

#include "Uint32.h"

namespace MixCore::math {

    struct Vector2f {

        float x, y;

        constexpr Vector2f(const float x, const float y) : x(x), y(y) {}


        //Vector operations

        //float specific
        [[nodiscard]] constexpr float xNDC() const {return x * 2.f - 1.f;}
        [[nodiscard]] constexpr float yNDC() const {return -(y * 2.f - 1.f);}

        [[nodiscard]] constexpr float dot(const Vector2f& other) const noexcept {
            return x * other.x + y * other.y;
        }

        [[nodiscard]] constexpr float cross(const Vector2f& other) const noexcept {
            return x * other.y - y * other.x;
        }

        [[nodiscard]] float length() const noexcept {
            return std::sqrt(x * x + y * y);
        }

        [[nodiscard]] float lengthSquared() const noexcept {
            return x * x + y * y;
        }


        [[nodiscard]] Vector2f normalized() const noexcept {
            const float length = this->length();
            if (length == 0.f) return {0.f, 0.f};
            return *this / length;
        }

        static float distance(const Vector2f& a, const Vector2f& b) noexcept {
            return (a - b).length();
        }

        //Arithmetic Operators
        constexpr Vector2f operator+(const Vector2f& other) const noexcept {
            return {x + other.x, y + other.y};
        }
        constexpr Vector2f operator-(const Vector2f& other) const noexcept {
            return {x - other.x, y - other.y};
        }
        constexpr Vector2f operator*(const float scalar) const noexcept {
            return {x * scalar, y * scalar};
        }
        constexpr Vector2f operator/(const float scalar) const noexcept {
            return {x / scalar, y / scalar};
        }

        //Compound Assignments
        Vector2f& operator+=(const Vector2f& other) noexcept {
            x += other.x; y += other.y; return *this;
        }
        Vector2f& operator-=(const Vector2f& other) noexcept {
            x -= other.x; y -= other.y; return *this;
        }
        Vector2f& operator*=(const float scalar) noexcept {
            x *= scalar; y *= scalar; return *this;
        }
        Vector2f& operator/=(const float scalar) noexcept {
            x /= scalar; y /= scalar; return *this;
        }

        // Comparison Operators
        constexpr bool operator==(const Vector2f& other) const noexcept {
            return x == other.x && y == other.y;
        }
        constexpr bool operator!=(const Vector2f& other) const noexcept {
            return !(*this == other);
        }
        constexpr bool operator<(const Vector2f& other) const noexcept {
            return x*x + y*y < other.x*other.x + other.y*other.y;
        }
        constexpr bool operator>(const Vector2f& other) const noexcept {
            return other < *this;
        }
        constexpr bool operator<=(const Vector2f& other) const noexcept {
            return other >= *this;
        }
        constexpr bool operator>=(const Vector2f& other) const noexcept {
            return !(*this < other);
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2f& v) {
            return os << "(" << v.x << ", " << v.y << ")";
        }

    };

    struct Vector2u {

        Uint32 x, y;

        constexpr Vector2u(const Uint32 x, const Uint32 y): x(x), y(y) {}
        constexpr Vector2u(const unsigned int x, const unsigned int y): x(Uint32(x)), y(Uint32(y)) {}
        constexpr Vector2u(const int x, const int y): x(Uint32(x)), y(Uint32(y)) {}

        [[nodiscard]] signed int signedX() const {return x.signedInt();}
        [[nodiscard]] signed int signedY() const {return y.signedInt();}

        [[nodiscard]] float length() const noexcept {
            return std::sqrt(static_cast<float>(x.raw()) * static_cast<float>(x.raw()) + static_cast<float>(y.raw()) * static_cast<float>(y.raw()));
        }

        [[nodiscard]] constexpr Uint32 lengthSquared() const noexcept {
            return x * x + y * y;
        }

        [[nodiscard]] constexpr Uint32 dot(const Vector2u& other) const noexcept {
            return x * other.x + y * other.y;
        }
        [[nodiscard]] constexpr int32_t cross(const Vector2u& other) const noexcept {
            return static_cast<int32_t>(x) * static_cast<int32_t>(other.y) - static_cast<int32_t>(y) * static_cast<int32_t>(other.x);
        }

        static float distance(const Vector2u& a, const Vector2u& b) noexcept {
            return (a - b).length();
        }

        //Comparison Operators
        constexpr bool operator==(const Vector2u& other) const noexcept {
            return x == other.x && y == other.y;
        }
        constexpr bool operator!=(const Vector2u& other) const noexcept {
            return !(*this == other);
        }
        constexpr bool operator<(const Vector2u& other) const noexcept {
            return x*x + y*y < other.x*other.x + other.y*other.y;
        }
        constexpr bool operator>(const Vector2u& other) const noexcept {
            return other < *this;
        }
        constexpr bool operator<=(const Vector2u& other) const noexcept {
            return other >= *this;
        }
        constexpr bool operator>=(const Vector2u& other) const noexcept {
            return !(*this < other);
        }


        //Arithmetic Operators

        constexpr Vector2u operator+(const Vector2u& other) const noexcept {
            return {x + other.x, y + other.y};
        }
        constexpr Vector2u operator-(const Vector2u& other) const noexcept {
            return {x - other.x, y - other.y};
        }
        constexpr Vector2u operator*(const Uint32 scalar) const noexcept {
            return {x * scalar, y * scalar};
        }
        constexpr Vector2u operator/(const Uint32 scalar) const noexcept {
            return {x / scalar, y / scalar};
        }
        constexpr Vector2u operator%(const Uint32 scalar) const noexcept {
            return {x % scalar, y % scalar};
        }


        //Compound Assignments
        Vector2u& operator+=(const Vector2u& other) noexcept {
            x += other.x; y += other.y; return *this;
        }
        Vector2u& operator-=(const Vector2u& other) noexcept {
            x -= other.x; y -= other.y; return *this;
        }
        Vector2u& operator*=(const math::Uint32 scalar) noexcept {
            x *= scalar; y *= scalar; return *this;
        }
        Vector2u& operator/=(const Uint32 scalar) noexcept {
            x /= scalar; y /= scalar; return *this;
        }
        Vector2u& operator%=(const Uint32 scalar) noexcept {
            x %= scalar; y %= scalar; return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2u& v) {
            return os << "(" << v.x << ", " << v.y << ")";
        }

    };
}


