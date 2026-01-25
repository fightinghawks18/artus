//
// Created by fightinghawks18 on 1/24/26.
//

#ifdef __APPLE__
#define VK_USE_PLATFORM_METAL_EXT
#include "Artus/Graphics/Utils/Metal/Layer.h"
#endif

#include "Artus/Graphics/Surface.h"

#include <iostream>

namespace Artus::Graphics {
    Surface::Surface(Device& device, Core::Window* window) : mDevice(device), mWindow(window) {
        CreateSurface(window);
        CreateSwapchain(window);
        CreateSemaphores();
        CreateFences();
        CreateImageViews();
    }

    Surface::~Surface() {
        DestroyImageViews();
        DestroyFences();
        DestroySemaphores();
        DestroySwapchain();
        DestroySurface();
    }

    uint32_t Surface::AcquireNextImage(vk::Semaphore* outSemaphore) {
        auto waitResult = mDevice.GetVkDevice().waitForFences(1, &mInFlightFens[mFrameIdx].get(), true, UINT64_MAX);
        if (waitResult != vk::Result::eSuccess) {
            std::cerr << "Failed to wait for fences before acquiring new image! (Occurred on frame " << mFrameIdx << ")" << std::endl;
            return UINT32_MAX;
        }

        auto resetResult = mDevice.GetVkDevice().resetFences(1, &mInFlightFens[mFrameIdx].get());
        if (resetResult != vk::Result::eSuccess) {
            std::cerr << "Failed to reset fence before acquiring new image! (Occurred on frame " << mFrameIdx << ")" << std::endl;
            return UINT32_MAX;
        }

        uint32_t imageIndex = UINT32_MAX;
        vk::Result getImageResult = mDevice.GetVkDevice().acquireNextImageKHR(mSwapchain.get(), UINT64_MAX, mImageAvailableSems[mFrameIdx].get(), nullptr, &imageIndex);
        if (getImageResult == vk::Result::eErrorOutOfDateKHR || getImageResult == vk::Result::eSuboptimalKHR) {
            std::cout << "Swapchain is out of date or is suboptimal, resizing. (Occurred on frame " << mFrameIdx << ")" << std::endl;
            Rebuild();
            return AcquireNextImage(outSemaphore);
        }

        if (getImageResult != vk::Result::eSuccess) {
            std::cerr << "Failed to acquire new image from swapchain! (Occurred on frame " << mFrameIdx << ")" << std::endl;
            return UINT32_MAX;
        }

        *outSemaphore = mImageAvailableSems[mFrameIdx].get();
        return imageIndex;
    }

    bool Surface::PresentDrawn(uint32_t imageIndex, vk::CommandBuffer commandBuffer, vk::Semaphore waitSemaphore) {
        std::vector<vk::PipelineStageFlags> waitDstStageMask = {
            vk::PipelineStageFlagBits::eAllCommands
        };

        vk::SubmitInfo submitInfo = {};
        submitInfo.setCommandBuffers(commandBuffer)
            .setWaitDstStageMask(waitDstStageMask)
            .setWaitSemaphores(waitSemaphore)
            .setSignalSemaphores(mRenderFinishedSems[mFrameIdx].get());

        auto submitResult = mDevice.GetVkGraphicsQueue().submit(1, &submitInfo, mInFlightFens[mFrameIdx].get());
        if (submitResult != vk::Result::eSuccess) {
            std::cerr << "Failed to submit graphics to swapchain! (Occurred on frame " << mFrameIdx << ")" << std::endl;
            return false;
        }

        vk::PresentInfoKHR presentInfo = {};
        presentInfo.setImageIndices(imageIndex)
            .setSwapchains(mSwapchain.get())
            .setWaitSemaphores(mRenderFinishedSems[mFrameIdx].get());

        auto presentResult = mDevice.GetVkGraphicsQueue().presentKHR(&presentInfo);
        if (presentResult != vk::Result::eSuccess) {
            std::cerr << "Failed to present graphics to swapchain! (Occurred on frame " << mFrameIdx << ")" << std::endl;
            return false;
        }

        mFrameIdx = (mFrameIdx + 1) % 2;
        return true;
    }

    void Surface::CreateSurface(Core::Window* window) {
#ifdef __APPLE__
        vk::MetalSurfaceCreateInfoEXT surfaceInfo = {};
        surfaceInfo.setPLayer(Metal::CreateMetalLayer(window));
        mSurface = mDevice.GetVkInstance().createMetalSurfaceEXTUnique(surfaceInfo);
#endif
    }

    void Surface::CreateSwapchain(Core::Window* window) {
        // Get present mode
        auto presentMode = vk::PresentModeKHR::eImmediate; // No "None" enum available
        for (const auto& mode : mDevice.GetVkPhysicalDevice().getSurfacePresentModesKHR(mSurface.get())) {
            if (mode == vk::PresentModeKHR::eMailbox) {
                presentMode = mode; // Mailbox is most recommended due to it having the least latency
                break;
            }
        }

        if (presentMode == vk::PresentModeKHR::eImmediate)
            presentMode = vk::PresentModeKHR::eFifo; // Fifo is always available, but not the best

        // Get Surface Format
        vk::SurfaceFormatKHR surfaceFormat = {};
        surfaceFormat.format = vk::Format::eUndefined;
        auto surfaceFormats = mDevice.GetVkPhysicalDevice().getSurfaceFormatsKHR(mSurface.get());
        for (const auto& format : surfaceFormats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb
                && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                surfaceFormat = format; // BGRA8_SRGB is common, so we use SRGB_nonlinear so that we don't reconvert SRGB swapchain data
                break;
            }
        }

        if (surfaceFormat.format == vk::Format::eUndefined)
            surfaceFormat = surfaceFormats[0];

        // Get Surface Extent
        const auto surfaceCapabilities = mDevice.GetVkPhysicalDevice().getSurfaceCapabilitiesKHR(mSurface.get());
        auto surfaceExtent = surfaceCapabilities.currentExtent;

        const auto windowSize = window->GetWindowSize();

        // Window systems may allow the app to define the surface extent,
        // and is within the min/max bounds.
        // Otherwise, it is controlled by the window's size
        if (surfaceExtent.width == 0xFFFFFFFF || surfaceExtent.height == 0xFFFFFFFF) {
            surfaceExtent.setWidth(std::clamp(
                    static_cast<uint32_t>(windowSize.width),
                    surfaceCapabilities.minImageExtent.width,
                    surfaceCapabilities.maxImageExtent.width));
            surfaceExtent.setHeight(std::clamp(
                    static_cast<uint32_t>(windowSize.height),
                    surfaceCapabilities.minImageExtent.height,
                    surfaceCapabilities.maxImageExtent.height));
        }

        vk::SwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.setImageFormat(surfaceFormat.format)
            .setImageColorSpace(surfaceFormat.colorSpace)
            .setImageArrayLayers(1)
            .setImageSharingMode(vk::SharingMode::eExclusive)
            .setImageExtent(surfaceExtent)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setMinImageCount(2)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setClipped(true)
            .setPresentMode(presentMode)
            .setPreTransform(vk::SurfaceTransformFlagBitsKHR::eIdentity)
            .setSurface(mSurface.get());

        if (mSwapchain)
            swapchainInfo.setOldSwapchain(mSwapchain.get());

        auto swapchain = mDevice.GetVkDevice().createSwapchainKHRUnique(swapchainInfo);
        if (mSwapchain)
            DestroySwapchain();
        mSwapchain = std::move(swapchain);

        mSurfaceExtent = surfaceExtent;
        mSurfaceFormat = surfaceFormat;
    }

    void Surface::CreateSemaphores() {
        constexpr vk::SemaphoreCreateInfo semaphoreInfo = {};
        for (uint32_t i = 0; i < 2; i++) {
            mImageAvailableSems.push_back(mDevice.GetVkDevice().createSemaphoreUnique(semaphoreInfo));
            mRenderFinishedSems.push_back(mDevice.GetVkDevice().createSemaphoreUnique(semaphoreInfo));
        }
    }

    void Surface::CreateFences() {
        vk::FenceCreateInfo fenceInfo = {};
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (uint32_t i = 0; i < 2; i++) {
            mInFlightFens.push_back(mDevice.GetVkDevice().createFenceUnique(fenceInfo));
        }
    }

    void Surface::CreateImageViews() {
        mImages = mDevice.GetVkDevice().getSwapchainImagesKHR(mSwapchain.get());

        for (const auto& image : mImages) {
            vk::ImageSubresourceRange subresourceRange = {};
            subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setBaseMipLevel(0)
                .setLevelCount(1);

            vk::ComponentMapping componentMapping = {};
            componentMapping.setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setA(vk::ComponentSwizzle::eIdentity);

            vk::ImageViewCreateInfo imageViewInfo = {};
            imageViewInfo.setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(mSurfaceFormat.format)
                .setSubresourceRange(subresourceRange)
                .setComponents(componentMapping);

            mImageViews.push_back(mDevice.GetVkDevice().createImageViewUnique(imageViewInfo));
        }
    }

    void Surface::DestroyImageViews() { mImageViews.clear(); }
    void Surface::DestroyFences() { mInFlightFens.clear(); }
    void Surface::DestroySemaphores() {
        mImageAvailableSems.clear();
        mRenderFinishedSems.clear();
    }

    void Surface::DestroySwapchain() { mSwapchain.reset(); }
    void Surface::DestroySurface() { mSurface.reset(); }

    void Surface::Rebuild() {
        DestroyImageViews();
        DestroySemaphores();
        DestroyFences();
        CreateSwapchain(mWindow);
        CreateSemaphores();
        CreateFences();
        CreateImageViews();
    }
} // namespace Artus::Graphics
