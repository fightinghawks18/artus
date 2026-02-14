//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_GRAPHICS_FLAGS_H
#define ARTUS_GRAPHICS_FLAGS_H

#include <cstdint>

namespace Artus::Graphics::Flags {
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

    enum class ImageAspect {
        None = 0,
        Color = 1 << 0,
        Depth = 1 << 1,
        Stencil = 1 << 2
    };

    inline ImageAspect operator|(const ImageAspect i0, const ImageAspect i1) {
        return static_cast<ImageAspect>(static_cast<uint32_t>(i0) | static_cast<uint32_t>(i1));
    }

    inline ImageAspect operator&(const ImageAspect i0, const ImageAspect i1) {
        return static_cast<ImageAspect>(static_cast<uint32_t>(i0) & static_cast<uint32_t>(i1));
    }
}

#endif // ARTUS_GRAPHICS_FLAGS_H