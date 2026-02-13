//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ISURFACE_H
#define ARTUS_ISURFACE_H

#include <Artus/Core/Window.h>

#include "Artus/Graphics/RHI/Types/Common.h"
#include "Resources/ICommandEncoder.h"

namespace Artus::Graphics::RHI {
    struct SurfaceDesc {
        Core::Window* window;
    };

    struct SurfaceFrameInfo {
        IImage* image;
        IImageView* view;
    };

    class ISurface {
    public:
        virtual ~ISurface() = default;

        virtual SurfaceFrameInfo PrepareFrame() = 0;
        virtual void PresentFrame(ICommandEncoder* encoder) = 0;

        [[nodiscard]] virtual Rectangle GetRectangle() const = 0;
        [[nodiscard]] virtual uint32_t GetImageIndex() const = 0;
        [[nodiscard]] virtual uint32_t GetFrameIndex() const = 0;

    };
}

#endif // ARTUS_ISURFACE_H
