//
// Created by fightinghawks18 on 2/11/26.
//


#ifndef ARTUS_COMMON_H
#define ARTUS_COMMON_H

namespace Artus::Graphics {
    struct Rectangle {
        int32_t x, y;
        uint32_t width, height;
    };

    struct Viewport {
        float x, y, width, height;
        float minDepth, maxDepth;
    };
}

#endif // ARTUS_COMMON_H