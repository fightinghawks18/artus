//
// Created by fightinghawks18 on 1/24/2026.
//

#ifndef ARTUS_SURFACE_H
#define ARTUS_SURFACE_H

#include "Artus/Core/Window.h"
#include "Resources/Image.h"
#include "Resources/ImageView.h"
#include "Utils/Common/Format.h"

#include <vulkan/vulkan.hpp>
#include <functional>

#include "../RHI/Types/Common.h"
#include "Artus/Graphics/RHI/ISurface.h"

namespace Artus::Graphics::Vulkan {
    class Device;

    class Surface : public RHI::ISurface {
    public:
        explicit Surface(Device& device, const RHI::SurfaceCreateDesc& desc);
        ~Surface() override;

        void OnResize(const std::function<void()>& onResizeFun) override;

        RHI::SurfaceFrameInfo PrepareFrame() override;
        void PresentFrame(RHI::ICommandEncoder* encoder) override;

        [[nodiscard]] RHI::Rectangle GetRectangle() const override {
            return {0, 0, mSurfaceExtent.width, mSurfaceExtent.height};
        }

        [[nodiscard]] vk::SurfaceFormatKHR GetVulkanSurfaceFormat() const { return mSurfaceFormat; }
        [[nodiscard]] vk::Extent2D GetVulkanExtent() const { return mSurfaceExtent; }
        [[nodiscard]] Image* GetVulkanImage(const uint32_t index) const { return mColorImages[index].get(); }
        [[nodiscard]] ImageView* GetVulkanImageView(const uint32_t index) const { return mColorImageViews[index].get(); }
        [[nodiscard]] uint32_t GetImageIndex() const override { return mImageIndex; }
        [[nodiscard]] uint32_t GetFrameIndex() const override { return mFrameIdx; }
        [[nodiscard]] RHI::Format GetFormat() const override { return FromVkFormat(mSurfaceFormat.format); }

    private:
        Device& mDevice;
        Core::Window* mWindow;

        std::function<void()> mOnResizeFun;

        vk::UniqueSwapchainKHR mSwapchain;
        vk::UniqueSurfaceKHR mSurface;
        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::Extent2D mSurfaceExtent;

        std::vector<vk::UniqueSemaphore> mImageAvailableSems;
        std::vector<vk::UniqueSemaphore> mRenderFinishedSems;
        std::vector<vk::UniqueFence> mInFlightFens;

        std::vector<std::unique_ptr<Image>> mColorImages;
        std::vector<std::unique_ptr<ImageView>> mColorImageViews;
        std::vector<std::unique_ptr<Image>> mDepthImages;
        std::vector<std::unique_ptr<ImageView>> mDepthImageViews;

        uint32_t mFrameIdx = 0;
        uint32_t mImageIndex = 0;

        void CreateSurface(Core::Window* window);
        void CreateSwapchain(Core::Window* window);
        void CreateSemaphores();
        void CreateFences();
        void CreateImages();
        void CreateImageViews();

        void DestroyImages();
        void DestroyImageViews();
        void DestroyFences();
        void DestroySemaphores();
        void DestroySwapchain();
        void DestroySurface();

        void Rebuild();
    };
} // namespace Artus::Graphics

#endif // ARTUS_SURFACE_H
