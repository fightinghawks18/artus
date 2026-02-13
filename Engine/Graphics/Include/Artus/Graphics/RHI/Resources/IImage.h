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
        None = 0,
        Color = 1 << 0,
        DepthStencil = 1 << 1,
        Shader = 1 << 2
    };

    inline ImageUsage operator|(const ImageUsage i0, const ImageUsage i1) {
        return static_cast<ImageUsage>(static_cast<uint32_t>(i0) | static_cast<uint32_t>(i1));
    }

    inline ImageUsage operator&(const ImageUsage i0, const ImageUsage i1) {
        return static_cast<ImageUsage>(static_cast<uint32_t>(i0) & static_cast<uint32_t>(i1));
    }

    struct ImageDesc {
        Format format;
        ImageType type;
        Extent3D extent;
        ImageUsage usage;
        uint32_t layerCount;
        uint32_t levelCount;
    };

    class IImage {
    public:
        virtual ~IImage() = default;
    };
}

#endif // ARTUS_IIMAGE_H
