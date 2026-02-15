//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_DEVICE_H
#define ARTUS_RENDER_DEVICE_H

#include "RenderContext.h"
#include "Artus/Core/Logger.h"

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

    enum class RenderStartResult {
        Ok = 0,
        Fail,
    };

    struct RenderState {
        RenderStartResult result;
        RenderContext context;

        explicit operator bool() const {
            return result == RenderStartResult::Ok;
        }

        RenderContext* operator->() {
            if (!*this) {
                AR_PANIC("Attempted to access failed render state!");
                return nullptr;
            }
            return &context;
        }
    };

    class RenderDevice {
    public:
        explicit RenderDevice(const RenderDeviceCreateDesc& desc);
        ~RenderDevice();

        RenderState StartRendering();
        void EndRendering(RenderState& state);

        [[nodiscard]] Handle<Graphics::Vulkan::Image> GetCurrentSurfaceColorImage() const { return mSurfaceColorImages[mImageIndex]; }
        [[nodiscard]] Handle<Graphics::Vulkan::ImageView> GetCurrentSurfaceColorImageView() const { return mSurfaceColorImageViews[mImageIndex]; }
        [[nodiscard]] Handle<Graphics::Vulkan::Image> GetCurrentSurfaceDepthImage() const { return mSurfaceDepthImages[mImageIndex]; }
        [[nodiscard]] Handle<Graphics::Vulkan::ImageView> GetCurrentSurfaceDepthImageView() const { return mSurfaceDepthImageViews[mImageIndex]; }

    protected:

        Graphics::Vulkan::Image* GetImage(Handle<Graphics::Vulkan::Image>& handle) const;
        Graphics::Vulkan::ImageView* GetImageView(Handle<Graphics::Vulkan::ImageView>& handle) const;
        Graphics::Vulkan::CommandEncoder* GetCurrentCommandEncoder() const;
        Graphics::Vulkan::Surface* GetMainSurface() const;

        friend class RenderContext;
    private:
        RenderAPI mRenderingApi;
        Core::Window* mMainWindow;
        std::unique_ptr<Graphics::Vulkan::Device> mRHI;
        std::unique_ptr<Graphics::Vulkan::Surface> mSurface;

        std::vector<std::unique_ptr<Graphics::Vulkan::CommandEncoder>> mCommandEncoders;
        std::vector<Handle<Graphics::Vulkan::Image>> mSurfaceColorImages;
        std::vector<Handle<Graphics::Vulkan::ImageView>> mSurfaceColorImageViews;
        std::vector<Handle<Graphics::Vulkan::Image>> mSurfaceDepthImages;
        std::vector<Handle<Graphics::Vulkan::ImageView>> mSurfaceDepthImageViews;

        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::Image>> mImageAllocator;
        std::unique_ptr<ResourceAllocator<Graphics::Vulkan::ImageView>> mImageViewAllocator;

        uint32_t mFrameIndex = 0;
        uint32_t mImageIndex = 0;
        bool mSurfaceReady = false;

        void CreateRenderDevice();
        void CreateAllocators();
        void CreateMainSurface();
        void CreateResources();
        void CreateSurfaceResources();
        void DestroySurfaceResources();
        void DestroyResources();
        void DestroyMainSurface();
        void DestroyAllocators();
        void DestroyRenderDevice();
    };
}

#endif // ARTUS_RENDER_DEVICE_H