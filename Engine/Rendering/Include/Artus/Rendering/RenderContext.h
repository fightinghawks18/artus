//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_CONTEXT_H
#define ARTUS_RENDER_CONTEXT_H

#include <Artus/Graphics/RHI/IDevice.h>

namespace Artus::Rendering {
    struct FrameContext {
        Graphics::RHI::IDevice* device;
        Graphics::RHI::ICommandEncoder* encoder;
        Graphics::RHI::ISurface* surface;
        Graphics::RHI::SurfaceFrameInfo surfaceFrameInfo;
        uint32_t frame;
    };

    class RenderContext {
    public:
        explicit RenderContext(const FrameContext& context);
        ~RenderContext();

        void StartPass();
        void EndPass();

        [[nodiscard]] Graphics::RHI::ICommandEncoder* GetRawCmd() const { return mContext.encoder; }
        [[nodiscard]] const FrameContext& GetFrameContext() const { return mContext; }

    private:
        FrameContext mContext;
    };
}

#endif // ARTUS_RENDER_CONTEXT_H