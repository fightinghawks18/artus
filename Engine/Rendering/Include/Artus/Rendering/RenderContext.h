//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_CONTEXT_H
#define ARTUS_RENDER_CONTEXT_H

#include <Artus/Graphics/Vulkan/Surface.h>
#include <Artus/Graphics/Vulkan/Device.h>

namespace Artus::Rendering {
    struct FrameContext {
        Graphics::Vulkan::Device* device;
        Graphics::Vulkan::CommandEncoder* encoder;
        Graphics::Vulkan::Surface* surface;
        Graphics::Structs::SurfaceFrameInfo surfaceFrameInfo;
        uint32_t frame;
    };

    class RenderContext {
    public:
        explicit RenderContext(const FrameContext& context);
        ~RenderContext();

        void StartPass();
        void EndPass();

        [[nodiscard]] Graphics::Vulkan::CommandEncoder* GetRawCmd() const { return mContext.encoder; }
        [[nodiscard]] const FrameContext& GetFrameContext() const { return mContext; }

    private:
        FrameContext mContext;
    };
}

#endif // ARTUS_RENDER_CONTEXT_H