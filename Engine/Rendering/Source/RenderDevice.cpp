//
// Created by fightinghawks18 on 2/13/2026.
//

#include "Artus/Rendering/RenderDevice.h"

#include "Artus/Core/Logger.h"

#include <Artus/Graphics/Vulkan/Device.h>

namespace Artus::Rendering {
    RenderDevice::RenderDevice(const RenderDeviceCreateDesc& desc) : mRenderingApi(desc.renderingApi), mMainWindow(desc.mainWindow) {
        CreateRenderDevice();
        CreateMainSurface();
        CreateResources();
    }

    RenderDevice::~RenderDevice() {
        mSurface.reset();
        mRHI.reset();
    }

    RenderContext RenderDevice::StartRendering() {
        Graphics::Structs::SurfaceFrameInfo surfaceInfo = {};
        uint32_t retries = 0;
        while (retries < 3 && (!surfaceInfo.colorImage || !surfaceInfo.colorView)) {
            surfaceInfo = mSurface->PrepareFrame();
            ++retries;
        }

        if (retries >= 3) {
            AR_PANIC("RenderDevice failed to reattempt rendering after 3 tries!");
            throw std::runtime_error("Retry render failure");
        }

        FrameContext ctx = {
            .device = mRHI.get(),
            .encoder = mCommandEncoders[mFrameIndex].get(),
            .surface = mSurface.get(),
            .surfaceFrameInfo = surfaceInfo,
            .frame = mFrameIndex
        };

        ctx.encoder->Start();
        ctx.encoder->MakeImageRenderable(ctx.surfaceFrameInfo.colorImage);
        return RenderContext(ctx);
    }

    void RenderDevice::EndRendering(const RenderContext& ctx) {
        auto& frameCtx = ctx.GetFrameContext();
        frameCtx.encoder->MakeImagePresentable(frameCtx.surfaceFrameInfo.colorImage);
        frameCtx.encoder->End();
        mSurface->PresentFrame(ctx.GetRawCmd());
        mFrameIndex = (mFrameIndex + 1) % 2;
    }

    void RenderDevice::CreateRenderDevice() {
        if (mRenderingApi == RenderAPI::Vulkan) {
            mRHI = std::make_unique<Graphics::Vulkan::Device>();
        }
    }

    void RenderDevice::CreateMainSurface() {
        mSurface = std::unique_ptr<Graphics::Vulkan::Surface>(mRHI->CreateSurface({ .window = mMainWindow }));
    }

    void RenderDevice::CreateResources() {
        for (auto& encoder : mRHI->CreateCommandEncoders(2)) {
            mCommandEncoders.push_back(std::unique_ptr<Graphics::Vulkan::CommandEncoder>(std::move(encoder)));
        }
    }
}