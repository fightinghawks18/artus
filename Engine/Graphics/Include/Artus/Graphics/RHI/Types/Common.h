//
// Created by fightinghawks18 on 2/11/26.
//


#ifndef ARTUS_COMMON_H
#define ARTUS_COMMON_H

#include <cstdint>

namespace Artus::Graphics::RHI {
    struct Rectangle {
        int32_t x, y;
        uint32_t width, height;
    };

    struct Extent {
        uint32_t width, height;
    };

    struct Extent3D {
        uint32_t width, height, depth;
    };

    struct Viewport {
        float x, y, width, height;
        float minDepth, maxDepth;
    };

    enum class ShaderStage {
        Vertex,
        Pixel,
        Compute
    };
}

#endif // ARTUS_COMMON_H
