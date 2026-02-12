//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_MATRIX4_H
#define ARTUS_MATRIX4_H
#include "Quaternion.h"
#include "Vector3.h"

#include <cmath>
#include <cstdint>

namespace Artus::Math {
    struct Matrix4 {
        float m[4][4];

        Matrix4() {
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;

            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = 0;

            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        static Matrix4 Translate(const float x, const float y, const float z) {
            Matrix4 matrix;
            matrix.m[0][3] = x;
            matrix.m[1][3] = y;
            matrix.m[2][3] = z;
            return matrix;
        }

        static Matrix4 RotateX(const float angle) {
            Matrix4 matrix;
            matrix.m[1][1] = std::cos(angle);
            matrix.m[1][2] = -std::sin(angle);
            matrix.m[2][1] = std::sin(angle);
            matrix.m[2][2] = std::cos(angle);
            return matrix;
        }

        static Matrix4 RotateY(const float angle) {
            Matrix4 matrix;
            matrix.m[0][0] = std::cos(angle);
            matrix.m[0][2] = std::sin(angle);
            matrix.m[2][0] = -std::sin(angle);
            matrix.m[2][2] = std::cos(angle);
            return matrix;
        }

        static Matrix4 RotateZ(const float angle) {
            Matrix4 matrix;
            matrix.m[0][0] = std::cos(angle);
            matrix.m[0][1] = -std::sin(angle);
            matrix.m[1][0] = std::sin(angle);
            matrix.m[1][1] = std::cos(angle);
            return matrix;
        }

        static Matrix4 QuaternionRotation(const Quaternion& quaternion) {
            const float x2 = quaternion.x * 2.0f;
            const float y2 = quaternion.y * 2.0f;
            const float z2 = quaternion.z * 2.0f;

            const float xx = quaternion.x * x2;
            const float xy = quaternion.x * y2;
            const float xz = quaternion.x * z2;

            const float yy = quaternion.y * y2;
            const float yz = quaternion.y * z2;

            const float zz = quaternion.z * z2;

            const float wx = quaternion.w * x2;
            const float wy = quaternion.w * y2;
            const float wz = quaternion.w * z2;

            Matrix4 matrix;

            matrix.m[0][0] = 1.0f - (yy + zz);
            matrix.m[0][1] = xy - wz;
            matrix.m[0][2] = xz + wy;
            matrix.m[0][3] = 0.0f;

            matrix.m[1][0] = xy + wz;
            matrix.m[1][1] = 1.0f - (xx + zz);
            matrix.m[1][2] = yz - wx;
            matrix.m[1][3] = 0.0f;

            matrix.m[2][0] = xz - wy;
            matrix.m[2][1] = yz + wx;
            matrix.m[2][2] = 1.0f - (xx + yy);
            matrix.m[2][3] = 0.0f;

            matrix.m[3][0] = 0.0f;
            matrix.m[3][1] = 0.0f;
            matrix.m[3][2] = 0.0f;
            matrix.m[3][3] = 1.0f;

            return matrix;
        }

        static Matrix4 Scale(const float x, const float y, const float z) {
            Matrix4 matrix;
            matrix.m[0][0] = x;
            matrix.m[1][1] = y;
            matrix.m[2][2] = z;
            return matrix;
        }

        static Matrix4 View(const Vector3& eye, const Vector3& target, const Vector3& up) {
            const auto eyeForward = (target - eye).Normalize();
            const auto eyeRight = eyeForward.Cross(up).Normalize();
            const auto eyeUp = eyeRight.Cross(eyeForward);

            Matrix4 matrix;
            matrix.m[0][0] = eyeRight.x;
            matrix.m[0][1] = eyeRight.y;
            matrix.m[0][2] = eyeRight.z;

            matrix.m[1][0] = eyeUp.x;
            matrix.m[1][1] = eyeUp.y;
            matrix.m[1][2] = eyeUp.z;

            matrix.m[2][0] = -eyeForward.x;
            matrix.m[2][1] = -eyeForward.y;
            matrix.m[2][2] = -eyeForward.z;

            matrix.m[0][3] = -eyeRight.Dot(eye);
            matrix.m[1][3] = -eyeUp.Dot(eye);
            matrix.m[2][3] = eyeForward.Dot(eye);

            matrix.m[3][3] = 1.0f;
            return matrix;
        }

        static Matrix4 Perspective(const float fov, const float aspect, const float near, const float far) {
            const float tanHalfFov = std::tan(fov / 2.0f);

            const float xZoom = 1.0f / (aspect * tanHalfFov);
            const float yZoom = 1.0f / tanHalfFov;
            const float coefficient = -far / (far - near);
            const float constantOffset = -(far * near) / (far - near);

            Matrix4 matrix;
            matrix.m[0][0] = xZoom;
            matrix.m[1][1] = yZoom;
            matrix.m[2][2] = coefficient;
            matrix.m[2][3] = constantOffset;
            matrix.m[3][2] = -1.0f;
            matrix.m[3][3] = 0.0f;
            return matrix;
        }

        Matrix4 Transpose() const {
            Matrix4 result;
            for (uint32_t i = 0; i < 4; i++) { for (uint32_t j = 0; j < 4; j++) { result.m[i][j] = m[j][i]; } }
            return result;
        }
    };

    inline Matrix4 operator*(const Matrix4& m0, const Matrix4& m1) {
        Matrix4 matrix;
        for (uint32_t i = 0; i < 4; i++) {
            for (uint32_t j = 0; j < 4; j++) {
                matrix.m[i][j] = m0.m[i][0] * m1.m[0][j] +
                    m0.m[i][1] * m1.m[1][j] +
                    m0.m[i][2] * m1.m[2][j] +
                    m0.m[i][3] * m1.m[3][j];
            }
        }
        return matrix;
    }
}

#endif
