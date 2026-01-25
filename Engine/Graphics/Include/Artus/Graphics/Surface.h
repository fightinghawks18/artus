//
// Created by fightinghawks18 on 1/24/26.
//

#ifndef ARTUS_SURFACE_H
#define ARTUS_SURFACE_H

#include "Device.h"

#include "Artus/Core/Platform.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class Surface {
    public:
        explicit Surface(Device& device, Core::Window* window);
        ~Surface();

        uint32_t AcquireNextImage(vk::Semaphore* outSemaphore);
        bool PresentDrawn(uint32_t imageIndex, vk::CommandBuffer commandBuffer, vk::Semaphore waitSemaphore);

        [[nodiscard]] vk::SurfaceFormatKHR GetVkSurfaceFormat() { return mSurfaceFormat; }
        [[nodiscard]] vk::Extent2D GetVkExtent() const { return mSurfaceExtent; }
        [[nodiscard]] vk::Image GetVkImage(const uint32_t index) const { return mImages[index]; }
        [[nodiscard]] vk::ImageView GetVkImageView(const uint32_t index) { return mImageViews[index].get(); }
        [[nodiscard]] uint32_t GetFrameIndex() const { return mFrameIdx; }
    private:
        Device& mDevice;
        Core::Window* mWindow;

        vk::UniqueSwapchainKHR mSwapchain;
        vk::UniqueSurfaceKHR mSurface;
        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::Extent2D mSurfaceExtent;

        std::vector<vk::UniqueSemaphore> mImageAvailableSems;
        std::vector<vk::UniqueSemaphore> mRenderFinishedSems;
        std::vector<vk::UniqueFence> mInFlightFens;

        std::vector<vk::Image> mImages;
        std::vector<vk::UniqueImageView> mImageViews;

        uint32_t mFrameIdx = 0;

        void CreateSurface(Core::Window* window);
        void CreateSwapchain(Core::Window* window);
        void CreateSemaphores();
        void CreateFences();
        void CreateImageViews();

        void DestroyImageViews();
        void DestroyFences();
        void DestroySemaphores();
        void DestroySwapchain();
        void DestroySurface();

        void Rebuild();
    };
}

#endif // ARTUS_SURFACE_H
