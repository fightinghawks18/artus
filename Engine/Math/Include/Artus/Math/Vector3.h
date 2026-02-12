//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_VECTOR3_H
#define ARTUS_VECTOR3_H

#include <cmath>

namespace Artus::Math {
    struct Vector3 {
        float x, y, z;

        explicit Vector3(const float x = 0, const float y = 0, const float z = 0) : x(x), y(y), z(z) {
        }

        Vector3() : x(0), y(0), z(0) {
        }

        static Vector3 Forward() { return Vector3{0, 0, 1}; }
        static Vector3 Right() { return Vector3{1}; }
        static Vector3 Up() { return Vector3{0, 1}; }

        [[nodiscard]] Vector3 Cross(const Vector3& v) const {
            const float xc = y * v.z - z * v.y;
            const float yc = z * v.x - x * v.z;
            const float zc = x * v.y - y * v.x;
            return Vector3{xc, yc, zc};
        }

        [[nodiscard]] float Dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }

        [[nodiscard]] float Length() const { return std::sqrtf(x * x + y * y + z * z); }

        Vector3& Normalize() {
            const auto normalized = Normalized();
            x = normalized.x;
            y = normalized.y;
            z = normalized.z;
            return *this;
        }

        [[nodiscard]] Vector3 Normalized() const {
            auto length = Length();
            if (length <= 0.0001f)
                return *this;
            return Vector3{x / length, y / length, z / length};
        }
    };

    inline Vector3 operator+(const Vector3& v0, const Vector3& v1) {
        return Vector3{v0.x + v1.x, v0.y + v1.y, v0.z + v1.z};
    }

    inline Vector3& operator+=(Vector3& v0, const Vector3& v1) {
        v0 = v0 + v1;
        return v0;
    }

    inline Vector3 operator-(const Vector3& v0, const Vector3& v1) {
        return Vector3{v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};
    }

    inline Vector3& operator-=(Vector3& v0, const Vector3& v1) {
        v0 = v0 - v1;
        return v0;
    }

    inline Vector3 operator/(const Vector3& v0, const Vector3& v1) {
        return Vector3{v0.x / v1.x, v0.y / v1.y, v0.z / v1.z};
    }

    inline Vector3 operator/(const Vector3& v0, const float f1) { return Vector3{v0.x / f1, v0.y / f1, v0.z / f1}; }

    inline Vector3& operator/=(Vector3& v0, const Vector3& v1) {
        v0 = v0 / v1;
        return v0;
    }

    inline Vector3& operator/=(Vector3& v0, const float f1) {
        v0 = v0 / f1;
        return v0;
    }

    inline Vector3 operator*(const Vector3& v0, const Vector3& v1) {
        return Vector3{v0.x * v1.x, v0.y * v1.y, v0.z * v1.z};
    }

    inline Vector3 operator*(const Vector3& v0, const float f1) { return Vector3{v0.x * f1, v0.y * f1, v0.z * f1}; }

    inline Vector3& operator*=(Vector3& v0, const Vector3& v1) {
        v0 = v0 * v1;
        return v0;
    }

    inline Vector3& operator*=(Vector3& v0, const float f1) {
        v0 = v0 * f1;
        return v0;
    }

    inline Vector3 operator-(const Vector3& v0) { return Vector3{-v0.x, -v0.y, -v0.z}; }

    inline bool operator==(const Vector3& v0, const Vector3& v1) {
        return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z;
    }

    inline bool operator!=(const Vector3& v0, const Vector3& v1) { return !(v0 == v1); }
}

#endif // ARTUS_VECTOR3_H
