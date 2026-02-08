//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_MATRIX4_H
#define ARTUS_MATRIX4_H
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
            matrix.m[3][0] = x; // Tx
            matrix.m[3][1] = y; // Ty
            matrix.m[3][2] = x; // Tz
            return matrix;
        }

        static Matrix4 RotateX(const float angle) {
            Matrix4 matrix;
            matrix.m[1][1] = std::cos(angle); // Yy
            matrix.m[1][2] = -std::sin(angle); // Yz
            matrix.m[2][1] = std::sin(angle); // Zy
            matrix.m[2][2] = std::cos(angle); // Zz
            return matrix;
        }

        static Matrix4 RotateY(const float angle) {
            Matrix4 matrix;
            matrix.m[0][0] = std::cos(angle); // Xx
            matrix.m[0][2] = std::sin(angle); // Xz
            matrix.m[2][0] = -std::sin(angle); // Zx
            matrix.m[2][2] = std::cos(angle); // Zz
            return matrix;
        }

        static Matrix4 RotateZ(const float angle) {
            Matrix4 matrix;
            matrix.m[0][0] = std::cos(angle); // Xx
            matrix.m[1][0] = std::sin(angle); // Xy
            matrix.m[0][1] = -std::sin(angle); // Yx
            matrix.m[1][1] = std::cos(angle); // Yy
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
            auto forwardV = (target - eye).Normalize();
            auto rightV = forwardV.Cross(up).Normalize();
            auto upV = rightV.Cross(forwardV);

            Matrix4 matrix;
            matrix.m[0][0] = rightV.x;
            matrix.m[1][0] = rightV.y;
            matrix.m[2][0] = rightV.z;

            matrix.m[0][1] = upV.x;
            matrix.m[1][1] = upV.y;
            matrix.m[2][1] = upV.z;

            matrix.m[0][2] = -forwardV.x;
            matrix.m[1][2] = -forwardV.y;
            matrix.m[2][2] = -forwardV.z;

            matrix.m[3][0] = -rightV.Dot(eye);
            matrix.m[3][1] = -upV.Dot(eye);
            matrix.m[3][2] = forwardV.Dot(eye);
            return matrix;
        }

        static Matrix4 Perspective(const float fov, const float aspect, const float near, const float far) {
            Matrix4 matrix;

            float tanHalfFov = std::tan(fov / 2.0f);

            matrix.m[0][0] = 1.0f / (tanHalfFov * aspect); // Scales perspective X scale based on fov and aspect
            matrix.m[1][1] = 1.0f / tanHalfFov; // Scales perspective Y scale based on FOV
            matrix.m[2][2] = -(far + near) / (far - near);
            matrix.m[2][3] = -1;

            matrix.m[3][2] = -(2.0f * far * near) / (far - near);
            matrix.m[3][3] = 0.0f;

            return matrix;
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

#endif // ARTUS_MATRIX4_H