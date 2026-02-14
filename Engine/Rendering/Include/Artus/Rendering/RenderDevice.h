//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_DEVICE_H
#define ARTUS_RENDER_DEVICE_H

#include "RenderContext.h"

#include <Artus/Core/Window.h>
#include <Artus/Graphics/Vulkan/Device.h>

namespace Artus::Rendering {
    enum class RenderAPI {
        Vulkan
    };

    struct RenderDeviceCreateDesc {
        RenderAPI renderingApi;
        Core::Window* mainWindow;
    };

    class RenderDevice {
    public:
        explicit RenderDevice(const RenderDeviceCreateDesc& desc);
        ~RenderDevice();

        RenderContext StartRendering();
        void EndRendering(const RenderContext& ctx);

    private:
        RenderAPI mRenderingApi;
        Core::Window* mMainWindow;
        std::unique_ptr<Graphics::Vulkan::Device> mRHI;
        std::unique_ptr<Graphics::Vulkan::Surface> mSurface;

        std::vector<std::unique_ptr<Graphics::Vulkan::CommandEncoder>> mCommandEncoders;

        uint32_t mFrameIndex = 0;

        void CreateRenderDevice();
        void CreateMainSurface();
        void CreateResources();
    };
}

#endif // ARTUS_RENDER_DEVICE_H