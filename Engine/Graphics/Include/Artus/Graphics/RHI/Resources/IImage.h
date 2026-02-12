//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IIMAGE_H
#define ARTUS_IIMAGE_H

#include "../Types/Format.h"
#include "../Types/Common.h"

namespace Artus::Graphics::RHI {
    enum class ImageType {
        Image2D,
        Image3D
    };

    enum class ImageUsage {
        Color,
        Depth,
        Stencil,
        Shader
    };

    struct ImageDesc {
        Format format;
        ImageType type;
        Rectangle rect;
        ImageUsage usage;
        uint32_t layerCount;
        uint32_t levelCount;
    };

    class IImage {
    public:
        virtual ~IImage() = 0;
    };
}

#endif // ARTUS_IIMAGE_H