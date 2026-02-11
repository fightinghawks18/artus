//
// Created by fightinghawks18 on 2/11/26.
//


#ifndef ARTUS_COLOR_H
#define ARTUS_COLOR_H

namespace Artus::Math {
    struct Color {
        float r, g, b, a;

        explicit Color(const float r, const float g, const float b, const float a = 1.0f) : r(r), g(g), b(b), a(a) {}
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

        void Mix(const Color& c1) {
            r = (r + c1.r) / 2;
            g = (g + c1.g) / 2;
            b = (b + c1.b) / 2;
            a = (a + c1.a) / 2;
        }
    };
    
    inline Color operator+(const Color& c0, const Color& c1) {
        return Color{c0.r + c1.r, c0.g + c1.g, c0.b + c1.b, c0.a + c1.a};
    }

    inline Color& operator+=(Color& c0, const Color& c1) {
        c0 = c0 + c1;
        return c0;
    }

    inline Color operator-(const Color& c0, const Color& c1) {
        return Color{c0.r - c1.r, c0.g - c1.g, c0.b - c1.b, c0.a - c1.a};
    }

    inline Color& operator-=(Color& c0, const Color& c1) {
        c0 = c0 - c1;
        return c0;
    }

    inline Color operator/(const Color& c0, const Color& c1) {
        return Color{c0.r / c1.r, c0.g / c1.g, c0.b / c1.b, c0.a / c1.a};
    }

    inline Color operator/(const Color& c0, const float f1) {
        return Color{c0.r / f1, c0.g / f1, c0.b / f1, c0.a / f1};
    }

    inline Color& operator/=(Color& c0, const Color& c1) {
        c0 = c0 / c1;
        return c0;
    }

    inline Color& operator/=(Color& c0, const float f1) {
        c0 = c0 / f1;
        return c0;
    }

    inline Color operator*(const Color& c0, const Color& c1) {
        return Color{c0.r * c1.r, c0.g * c1.g, c0.b * c1.b, c0.a * c1.a};
    }

    inline Color operator*(const Color& c0, const float f1) {
        return Color{c0.r * f1, c0.g * f1, c0.b * f1, c0.a * f1};
    }

    inline Color& operator*=(Color& c0, const Color& c1) {
        c0 = c0 * c1;
        return c0;
    }

    inline Color& operator*=(Color& c0, const float f1) {
        c0 = c0 * f1;
        return c0;
    }

    inline Color operator-(const Color& c0) {
        return Color{-c0.r, -c0.g, -c0.b, -c0.a};
    }

    inline bool operator==(const Color& c0, const Color& c1) {
        return c0.r == c1.r && c0.g == c1.g && c0.b == c1.b && c0.a == c1.a;
    }

    inline bool operator!=(const Color& c0, const Color& c1) {
        return !(c0 == c1);
    }
}

#endif // ARTUS_COLOR_H