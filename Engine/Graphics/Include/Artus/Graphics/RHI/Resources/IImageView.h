//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IIMAGE_VIEW_H
#define ARTUS_IIMAGE_VIEW_H

#include "IImage.h"

namespace Artus::Graphics::RHI {
    enum class ImageViewType {
        ImageView2D,
        ImageView3D
    };

    enum class ImageAspect {
        None = 0,
        Color,
        Depth,
        Stencil
    };

    inline ImageAspect operator|(const ImageAspect i0, const ImageAspect i1) {
        return static_cast<ImageAspect>(static_cast<uint32_t>(i0) | static_cast<uint32_t>(i1));
    }

    inline ImageAspect operator&(const ImageAspect i0, const ImageAspect i1) {
        return static_cast<ImageAspect>(static_cast<uint32_t>(i0) & static_cast<uint32_t>(i1));
    }

    struct ImageViewDesc {
        IImage* image;
        Format format;
        ImageViewType type;
        ImageAspect aspectMask;
        uint32_t baseLayer;
        uint32_t layerCount;
        uint32_t baseLevel;
        uint32_t levelCount;
    };

    class IImageView {
    public:
        virtual ~IImageView() = 0;
    };
}

#endif // ARTUS_IIMAGE_VIEW_H
