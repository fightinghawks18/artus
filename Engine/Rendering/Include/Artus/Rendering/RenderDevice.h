//
// Created by fightinghawks18 on 2/13/2026.
//


#ifndef ARTUS_RENDER_DEVICE_H
#define ARTUS_RENDER_DEVICE_H

#include "Material.h"
#include "MaterialGroup.h"
#include "RenderContext.h"
#include "Artus/Core/Logger.h"
#include "Utils/GraphicsAllocator.h"

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

        MaterialGroupHandle CreateMaterialGroup(const MaterialGroupCreateDesc& materialGroupCreateDesc) const;
        MaterialHandle CreateMaterial(const MaterialCreateDesc& materialCreateDesc);

        RenderState StartRendering();
        void EndRendering(RenderState& state);

        [[nodiscard]] ImageAndViewHandle GetCurrentSurfaceColorImage() const { return mSurfaceColorImages[mImageIndex]; }
        [[nodiscard]] ImageAndViewHandle GetCurrentSurfaceDepthImage() const { return mSurfaceDepthImages[mImageIndex]; }

    protected:

        Graphics::Vulkan::Image* GetImage(Handle<Graphics::Vulkan::Image>& handle) const;
        Graphics::Vulkan::ImageView* GetImageView(Handle<Graphics::Vulkan::ImageView>& handle) const;
        [[nodiscard]] Graphics::Vulkan::CommandEncoder* GetCurrentCommandEncoder() const;
        [[nodiscard]] Graphics::Vulkan::Surface* GetMainSurface() const;
        [[nodiscard]] Graphics::Vulkan::Device* GetRHI() const;
        [[nodiscard]] GraphicsAllocator* GetGraphicsAllocator() const;

        friend class RenderContext;
        friend class GraphicsAllocator;
    private:
        RenderAPI mRenderingApi;
        Core::Window* mMainWindow;
        std::unique_ptr<Graphics::Vulkan::Device> mRHI;
        std::unique_ptr<Graphics::Vulkan::Surface> mSurface;

        std::vector<std::unique_ptr<Graphics::Vulkan::CommandEncoder>> mCommandEncoders;
        std::vector<ImageAndViewHandle> mSurfaceColorImages;
        std::vector<ImageAndViewHandle> mSurfaceDepthImages;

        std::unique_ptr<GraphicsAllocator> mGraphicsAllocator;
        std::unique_ptr<ResourceAllocator<Material>> mMaterialAllocator;
        std::unique_ptr<ResourceAllocator<MaterialGroup>> mMaterialGroupAllocator;

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