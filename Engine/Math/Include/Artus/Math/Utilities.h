//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_MATH_UTILITIES_H
#define ARTUS_MATH_UTILITIES_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace Artus::Math {
    inline float AsRadians(const float degrees) { return degrees * (M_PI / 180); }

    inline float AsDegrees(const float radians) { return radians * (180 / M_PI); }
}

#endif // ARTUS_MATH_UTILITIES_H
