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

    class ISurface {
    public:
        virtual ~ISurface() = 0;

        virtual void PrepareFrame() = 0;
        virtual void PresentFrame(ICommandEncoder* encoder) = 0;

        [[nodiscard]] virtual Rectangle GetRectangle() const;
    };
}

#endif // ARTUS_ISURFACE_H
