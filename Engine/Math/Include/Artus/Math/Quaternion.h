//
// Created by fightinghawks18 on 2/8/2026.
//


#ifndef ARTUS_QUATERNION_H
#define ARTUS_QUATERNION_H
#include "Vector3.h"

namespace Artus::Math {
    struct Quaternion {
        float x, y, z, w;

        explicit Quaternion(const float x, const float y, const float z, const float w = 1.0f) : x(x), y(y), z(z), w(w) {}
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        static Quaternion FromAxisAngle(const Vector3& axis, const float angle) {
            const Vector3 normalizedAxis = axis.Normalized();
            const float halfAngle = angle / 2.0f;
            float halfAngleSin = std::sin(halfAngle);

            Quaternion quaternion;
            quaternion.x = normalizedAxis.x * halfAngleSin;
            quaternion.y = normalizedAxis.y * halfAngleSin;
            quaternion.z = normalizedAxis.z * halfAngleSin;
            quaternion.w = std::cos(halfAngle);
            return quaternion;
        }

        static Quaternion FromEuler(const Vector3& euler) {
            const float cosPitch = std::cos(euler.x / 2.0f);
            const float sinPitch = std::sin(euler.x / 2.0f);
            const float cosYaw = std::cos(euler.y / 2.0f);
            const float sinYaw = std::sin(euler.y / 2.0f);
            const float cosRoll = std::cos(euler.z * 0.5f);
            const float sinRoll = std::sin(euler.z / 2.0f);

            Quaternion quaternion;
            quaternion.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
            quaternion.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
            quaternion.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
            quaternion.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
            return quaternion;
        }

        [[nodiscard]] float Length() const {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        Quaternion& Normalize() {
            Quaternion quaternion = Normalized();
            x = quaternion.x;
            y = quaternion.y;
            z = quaternion.z;
            w = quaternion.w;
            return *this;
        }

        [[nodiscard]] Quaternion Normalized() const {
            const float length = Length();
            if (length <= 0.0001f)
                return *this;
            Quaternion quaternion;
            quaternion.x = x / length;
            quaternion.y = y / length;
            quaternion.z = z / length;
            quaternion.w = w / length;
            return quaternion;
        }
    };

    inline Quaternion operator*(const Quaternion& q0, const Quaternion& q1) {
        Quaternion quaternion;
        quaternion.x = q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y;
        quaternion.y = q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x;
        quaternion.z = q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w;
        quaternion.w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
        return quaternion;
    }

    inline Quaternion& operator*=(Quaternion& q0, const Quaternion& q1) {
        q0 = q0 * q1;
        return q0;
    }
}

#endif // ARTUS_QUATERNION_H