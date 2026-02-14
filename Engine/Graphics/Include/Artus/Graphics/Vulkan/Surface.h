//
// Created by fightinghawks18 on 1/24/2026.
//

#ifndef ARTUS_SURFACE_H
#define ARTUS_SURFACE_H

#include "Artus/Core/Window.h"
#include "Resources/CommandEncoder.h"
#include "Resources/Image.h"
#include "Resources/ImageView.h"
#include "Utils/Common/Format.h"

#include <vulkan/vulkan.hpp>
#include <functional>

namespace Artus::Graphics::Vulkan {
    class Device;

    class Surface {
    public:
        explicit Surface(Device& device, const Structs::SurfaceCreateDesc& desc);
        ~Surface();

        void OnResize(const std::function<void()>& onResizeFun);

        Structs::SurfaceFrameInfo PrepareFrame();
        void PresentFrame(CommandEncoder* encoder);

        [[nodiscard]] Structs::Rectangle GetRectangle() const {
            return {0, 0, mSurfaceExtent.width, mSurfaceExtent.height};
        }

        [[nodiscard]] vk::SurfaceFormatKHR GetVulkanSurfaceFormat() const { return mSurfaceFormat; }
        [[nodiscard]] vk::Extent2D GetVulkanExtent() const { return mSurfaceExtent; }
        [[nodiscard]] Image* GetVulkanImage(const uint32_t index) const { return mColorImages[index].get(); }
        [[nodiscard]] ImageView* GetVulkanImageView(const uint32_t index) const { return mColorImageViews[index].get(); }
        [[nodiscard]] uint32_t GetImageIndex() const { return mImageIndex; }
        [[nodiscard]] uint32_t GetFrameIndex() const { return mFrameIdx; }
        [[nodiscard]] Enums::Format GetFormat() const { return FromVkFormat(mSurfaceFormat.format); }

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
