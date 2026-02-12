//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_FORMAT_H
#define ARTUS_FORMAT_H

namespace Artus::Graphics::RHI {
    enum class Format {
        Unknown,

        RGBA8_SInt,
        RGBA8_SRgb,
        RGBA8_UNorm,
        RGBA8_SNorm,

        BGRA8_SInt,
        BGRA8_SRgb,
        BGRA8_UNorm,
        BGRA8_SNorm,

        D16_UNorm_S8_UInt,
        D32_SFloat_S8_UInt,
    };
}

#endif // ARTUS_FORMAT_H
