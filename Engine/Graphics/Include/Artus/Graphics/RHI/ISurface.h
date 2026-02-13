//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ISURFACE_H
#define ARTUS_ISURFACE_H

#include <Artus/Core/Window.h>

#include <functional>

#include "Artus/Graphics/RHI/Types/Common.h"
#include "Resources/ICommandEncoder.h"

namespace Artus::Graphics::RHI {
    struct SurfaceCreateDesc {
        Core::Window* window;
    };

    struct SurfaceFrameInfo {
        IImage* colorImage;
        IImageView* colorView;

        IImage* depthImage;
        IImageView* depthView;
    };

    class ISurface {
    public:
        virtual ~ISurface() = default;

        virtual void OnResize(const std::function<void()>& onResizeFun) = 0;

        virtual SurfaceFrameInfo PrepareFrame() = 0;
        virtual void PresentFrame(ICommandEncoder* encoder) = 0;

        [[nodiscard]] virtual Rectangle GetRectangle() const = 0;
        [[nodiscard]] virtual uint32_t GetImageIndex() const = 0;
        [[nodiscard]] virtual uint32_t GetFrameIndex() const = 0;
        [[nodiscard]] virtual Format GetFormat() const = 0;

    };
}

#endif // ARTUS_ISURFACE_H
