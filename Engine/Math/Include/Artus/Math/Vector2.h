//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_VECTOR2_H
#define ARTUS_VECTOR2_H

#include <cmath>

namespace Artus::Math {
    struct Vector2 {
        float x, y;

        explicit Vector2(const float x = 0, const float y = 0) : x(x), y(y) {}
        Vector2() : x(0), y(0) {}

        static Vector2 XUnit() { return Vector2{1, 0}; }
        static Vector2 YUnit() { return Vector2{1, 0}; }

        float Dot(const Vector2& v) const {
            return x * v.x + y * v.y;
        }

        float Magnitude() const {
            return std::sqrtf(x * x + y * y);
        }

        Vector2& Normalize() {
            const auto normalized = Normalized();
            x = normalized.x;
            y = normalized.y;
            return *this;
        }

        Vector2 Normalized() const {
            auto magnitude = Magnitude();
            if (magnitude <= 0.01f)
                return *this;
            return Vector2{x / magnitude, y / magnitude};
        }
    };

    inline Vector2 operator+(const Vector2& v0, const Vector2& v1) {
        return Vector2{v0.x + v1.x, v0.y + v1.y};
    }

    inline Vector2& operator+=(Vector2& v0, const Vector2& v1) {
        v0 = v0 + v1;
        return v0;
    }

    inline Vector2 operator-(const Vector2& v0, const Vector2& v1) {
        return Vector2{v0.x - v1.x, v0.y - v1.y};
    }

    inline Vector2& operator-=(Vector2& v0, const Vector2& v1) {
        v0 = v0 - v1;
        return v0;
    }

    inline Vector2 operator/(const Vector2& v0, const Vector2& v1) {
        return Vector2{v0.x / v1.x, v0.y / v1.y};
    }

    inline Vector2 operator/(const Vector2& v0, const float f1) {
        return Vector2{v0.x / f1, v0.y / f1};
    }

    inline Vector2& operator/=(Vector2& v0, const Vector2& v1) {
        v0 = v0 / v1;
        return v0;
    }

    inline Vector2 operator*(const Vector2& v0, const Vector2& v1) {
        return Vector2{v0.x * v1.x, v0.y * v1.y};
    }

    inline Vector2 operator*(const Vector2& v0, const float f1) {
        return Vector2{v0.x * f1, v0.y * f1};
    }

    inline Vector2& operator*=(Vector2& v0, const Vector2& v1) {
        v0 = v0 * v1;
        return v0;
    }

    inline Vector2 operator-(const Vector2& v0) {
        return Vector2{-v0.x, -v0.y};
    }

    inline bool operator==(const Vector2& v0, const Vector2& v1) {
        return v0.x == v1.x && v0.y == v1.y;
    }

    inline bool operator!=(const Vector2& v0, const Vector2& v1) {
        return !(v0 == v1);
    }
}

#endif // ARTUS_VECTOR2_H