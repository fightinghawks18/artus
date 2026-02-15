//
// Created by fightinghawks18 on 2/13/2026.
//

#include "Artus/Rendering/RenderDevice.h"

#include "Artus/Core/Logger.h"

#include <Artus/Graphics/Vulkan/Device.h>

namespace Artus::Rendering {
    RenderDevice::RenderDevice(const RenderDeviceCreateDesc& desc) : mRenderingApi(desc.renderingApi), mMainWindow(desc.mainWindow) {
        CreateRenderDevice();
        CreateAllocators();
        CreateMainSurface();
        CreateResources();
        CreateSurfaceResources();
        mSurfaceReady = true;
    }

    RenderDevice::~RenderDevice() {
        DestroyResources();
        DestroySurfaceResources();
        DestroyMainSurface();
        DestroyAllocators();
        DestroyRenderDevice();
    }

    RenderState RenderDevice::StartRendering() {
        if (!mSurfaceReady)
            return RenderState(RenderStartResult::Fail, RenderContext(*this));

        mSurface->PrepareFrame();
        mImageIndex = mSurface->GetImageIndex();

        const auto currentEncoder = GetCurrentCommandEncoder();
        auto currentSurfaceColorImageHandle = GetCurrentSurfaceColorImage();
        const auto surfaceColorImage = GetImage(currentSurfaceColorImageHandle);

        auto currentSurfaceDepthImageHandle = GetCurrentSurfaceDepthImage();
        const auto surfaceDepthImage = GetImage(currentSurfaceDepthImageHandle);

        currentEncoder->Start();
        currentEncoder->MakeImageRenderable(surfaceColorImage);
        currentEncoder->MakeImageDepthStencil(surfaceDepthImage);
        return RenderState(RenderStartResult::Ok, RenderContext(*this));
    }

    void RenderDevice::EndRendering(RenderState& state) {
        const auto currentEncoder = GetCurrentCommandEncoder();
        auto currentSurfaceColorImageHandle = GetCurrentSurfaceColorImage();
        const auto surfaceColorImage = GetImage(currentSurfaceColorImageHandle);

        currentEncoder->MakeImagePresentable(surfaceColorImage);
        currentEncoder->End();
        mSurface->PresentFrame(state->GetRawCmd());
        mFrameIndex = (mFrameIndex + 1) % 2;
    }

    Graphics::Vulkan::Image* RenderDevice::GetImage(Handle<Graphics::Vulkan::Image>& handle) const {
        return mImageAllocator->Get(handle);
    }

    Graphics::Vulkan::ImageView* RenderDevice::GetImageView(Handle<Graphics::Vulkan::ImageView>& handle) const {
        return mImageViewAllocator->Get(handle);
    }

    Graphics::Vulkan::CommandEncoder* RenderDevice::GetCurrentCommandEncoder() const {
        return mCommandEncoders[mFrameIndex].get();
    }

    Graphics::Vulkan::Surface* RenderDevice::GetMainSurface() const {
        return mSurface.get();
    }

    void RenderDevice::CreateRenderDevice() {
        if (mRenderingApi == RenderAPI::Vulkan) {
            mRHI = std::make_unique<Graphics::Vulkan::Device>();
        }
    }

    void RenderDevice::CreateAllocators() {
        mImageAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::Image>>();
        mImageViewAllocator = std::make_unique<ResourceAllocator<Graphics::Vulkan::ImageView>>();
    }

    void RenderDevice::CreateMainSurface() {
        mSurface = std::unique_ptr<Graphics::Vulkan::Surface>(mRHI->CreateSurface({ .window = mMainWindow }));

        mSurface->OnResize([&] {
            mSurfaceReady = false;
            DestroySurfaceResources();
            CreateSurfaceResources();
            mSurfaceReady = true;
        });
    }

    void RenderDevice::CreateResources() {
        for (auto& encoder : mRHI->CreateCommandEncoders(2)) {
            mCommandEncoders.push_back(std::unique_ptr<Graphics::Vulkan::CommandEncoder>(std::move(encoder)));
        }
    }

    void RenderDevice::CreateSurfaceResources() {
        for (const auto& image : mSurface->GetColorImages()) {
            mSurfaceColorImages.push_back(mImageAllocator->AllocateNonOwning(image));
        }

        for (const auto& view : mSurface->GetColorImageViews()) {
            mSurfaceColorImageViews.push_back(mImageViewAllocator->AllocateNonOwning(view));
        }

        for (const auto& image : mSurface->GetDepthImages()) {
            mSurfaceDepthImages.push_back(mImageAllocator->AllocateNonOwning(image));
        }

        for (const auto& view : mSurface->GetDepthImageViews()) {
            mSurfaceDepthImageViews.push_back(mImageViewAllocator->AllocateNonOwning(view));
        }
    }

    void RenderDevice::DestroySurfaceResources() {
        for (auto& image : mSurfaceColorImages) {
            mImageAllocator->Free(image);
        }
        mSurfaceColorImages.clear();

        for (auto& view : mSurfaceColorImageViews) {
            mImageViewAllocator->Free(view);
        }
        mSurfaceColorImageViews.clear();

        for (auto& image : mSurfaceDepthImages) {
            mImageAllocator->Free(image);
        }
        mSurfaceDepthImages.clear();

        for (auto& view : mSurfaceDepthImageViews) {
            mImageViewAllocator->Free(view);
        }
        mSurfaceDepthImageViews.clear();
    }

    void RenderDevice::DestroyResources() {
        mCommandEncoders.clear();
    }

    void RenderDevice::DestroyMainSurface() {
        mSurface.reset();
    }

    void RenderDevice::DestroyAllocators() {
        mImageViewAllocator.reset();
        mImageAllocator.reset();
    }

    void RenderDevice::DestroyRenderDevice() {
        mRHI.reset();
    }
}