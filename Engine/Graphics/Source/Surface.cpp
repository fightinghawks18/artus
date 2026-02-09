//
// Created by fightinghawks18 on 1/24/2026.
//

#ifdef __APPLE__
#define VK_USE_PLATFORM_METAL_EXT
#include "Artus/Graphics/Utils/Metal/Layer.h"
#endif
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "Artus/Graphics/Surface.h"

#include "Artus/Core/Logger.h"

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
        mDevice.GetVulkanDevice().waitIdle();

        DestroyImageViews();
        DestroyFences();
        DestroySemaphores();
        DestroySwapchain();
        DestroySurface();
    }

    uint32_t Surface::AcquireNextImage(vk::Semaphore* outSemaphore) {
        auto waitResult = mDevice.GetVulkanDevice().waitForFences(1, &mInFlightFens[mFrameIdx].get(), true, UINT64_MAX);
        if (waitResult != vk::Result::eSuccess) {
            AR_ERR("Failed to wait for fences before acquiring new image! (Occurred on frame {})", mFrameIdx);
            return 0;
        }

        auto resetResult = mDevice.GetVulkanDevice().resetFences(1, &mInFlightFens[mFrameIdx].get());
        if (resetResult != vk::Result::eSuccess) {
            AR_ERR("Failed to reset fence before acquiring new image! (Occurred on frame {})", mFrameIdx);
            return 0;
        }

        uint32_t imageIndex = UINT32_MAX;
        vk::Result getImageResult = mDevice.GetVulkanDevice().acquireNextImageKHR(
            mSwapchain.get(), UINT64_MAX, mImageAvailableSems[mFrameIdx].get(), nullptr, &imageIndex);
        if (getImageResult == vk::Result::eErrorOutOfDateKHR || getImageResult == vk::Result::eSuboptimalKHR) {
            AR_LOG("Swapchain is out of date or is suboptimal, resizing. (Occurred on frame {})", mFrameIdx);
            Rebuild();
            return AcquireNextImage(outSemaphore);
        }

        if (getImageResult != vk::Result::eSuccess) {
            AR_ERR("Failed to acquire new image from swapchain! (Occurred on frame {})", mFrameIdx);
            return 0;
        }

        *outSemaphore = mImageAvailableSems[mFrameIdx].get();
        return imageIndex;
    }

    bool Surface::PresentDrawn(uint32_t imageIndex, vk::CommandBuffer commandBuffer, vk::Semaphore waitSemaphore) {
        std::vector<vk::PipelineStageFlags> waitDstStageMask = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

        vk::SubmitInfo submitInfo = {};
        submitInfo.setCommandBuffers(commandBuffer)
            .setWaitDstStageMask(waitDstStageMask)
            .setWaitSemaphores(waitSemaphore)
            .setSignalSemaphores(mRenderFinishedSems[mFrameIdx].get());

        auto submitResult = mDevice.GetVulkanGraphicsQueue().submit(1, &submitInfo, mInFlightFens[mFrameIdx].get());
        if (submitResult != vk::Result::eSuccess) {
            AR_ERR("Failed to submit graphics to swapchain! (Occurred on frame {})", mFrameIdx);
            return false;
        }

        vk::PresentInfoKHR presentInfo = {};
        presentInfo.setImageIndices(imageIndex)
            .setSwapchains(mSwapchain.get())
            .setWaitSemaphores(mRenderFinishedSems[mFrameIdx].get());

        auto presentResult = mDevice.GetVulkanGraphicsQueue().presentKHR(&presentInfo);
        if (presentResult != vk::Result::eSuccess) {
            AR_ERR("Failed to present graphics to swapchain! (Occurred on frame {})", mFrameIdx);
            return false;
        }

        mFrameIdx = (mFrameIdx + 1) % 2;
        return true;
    }

    void Surface::CreateSurface(Core::Window* window) {
#ifdef __APPLE__
        vk::MetalSurfaceCreateInfoEXT surfaceInfo = {};
        surfaceInfo.setPLayer(Metal::CreateMetalLayer(window));
        mSurface = mDevice.GetVulkanInstance().createMetalSurfaceEXTUnique(surfaceInfo);
#endif
#ifdef _WIN32
        vk::Win32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.setHwnd(static_cast<HWND>(window->GetHandle().handle)).setHinstance(GetModuleHandle(nullptr));
        mSurface = mDevice.GetVulkanInstance().createWin32SurfaceKHRUnique(surfaceInfo);
#endif
    }

    void Surface::CreateSwapchain(Core::Window* window) {
        // Get present mode
        auto presentMode = vk::PresentModeKHR::eImmediate; // No "None" enum available
        for (const auto& mode : mDevice.GetVulkanPhysicalDevice().getSurfacePresentModesKHR(mSurface.get())) {
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
        auto surfaceFormats = mDevice.GetVulkanPhysicalDevice().getSurfaceFormatsKHR(mSurface.get());
        for (const auto& format : surfaceFormats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                surfaceFormat = format; // BGRA8_SRGB is common, so we use SRGB_nonlinear so that we don't reconvert
                                        // SRGB swapchain data
                break;
            }
        }

        if (surfaceFormat.format == vk::Format::eUndefined)
            surfaceFormat = surfaceFormats[0];

        // Get Surface Extent
        const auto surfaceCapabilities = mDevice.GetVulkanPhysicalDevice().getSurfaceCapabilitiesKHR(mSurface.get());
        auto surfaceExtent = surfaceCapabilities.currentExtent;

        const auto windowSize = window->GetSize();

        // Window systems may allow the app to define the surface extent,
        // and is within the min/max bounds.
        // Otherwise, it is controlled by the window's size
        if (surfaceExtent.width == 0xFFFFFFFF || surfaceExtent.height == 0xFFFFFFFF) {
            surfaceExtent.setWidth(std::clamp(static_cast<uint32_t>(windowSize.width),
                                              surfaceCapabilities.minImageExtent.width,
                                              surfaceCapabilities.maxImageExtent.width));
            surfaceExtent.setHeight(std::clamp(static_cast<uint32_t>(windowSize.height),
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

        auto swapchain = mDevice.GetVulkanDevice().createSwapchainKHRUnique(swapchainInfo);
        if (mSwapchain)
            DestroySwapchain();
        mSwapchain = std::move(swapchain);

        mSurfaceExtent = surfaceExtent;
        mSurfaceFormat = surfaceFormat;
    }

    void Surface::CreateSemaphores() {
        constexpr vk::SemaphoreCreateInfo semaphoreInfo = {};
        for (uint32_t i = 0; i < 2; i++) {
            mImageAvailableSems.push_back(mDevice.GetVulkanDevice().createSemaphoreUnique(semaphoreInfo));
            mRenderFinishedSems.push_back(mDevice.GetVulkanDevice().createSemaphoreUnique(semaphoreInfo));
        }
    }

    void Surface::CreateFences() {
        vk::FenceCreateInfo fenceInfo = {};
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (uint32_t i = 0; i < 2; i++) {
            mInFlightFens.push_back(mDevice.GetVulkanDevice().createFenceUnique(fenceInfo));
        }
    }

    void Surface::CreateImageViews() {
        mImages.clear();
        for (const auto& image : mDevice.GetVulkanDevice().getSwapchainImagesKHR(mSwapchain.get())) {
            auto img = std::make_unique<Image>(mDevice, image);

            ImageViewDesc imageViewDesc = {
                .format = mSurfaceFormat.format,
                .type = vk::ImageViewType::e2D,
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .baseLayer = 0,
                .layerCount = 1,
                .baseLevel = 0,
                .levelCount = 1,
                .redComponent = vk::ComponentSwizzle::eIdentity,
                .greenComponent = vk::ComponentSwizzle::eIdentity,
                .blueComponent = vk::ComponentSwizzle::eIdentity,
                .alphaComponent = vk::ComponentSwizzle::eIdentity
            };

            mImageViews.push_back(std::make_unique<ImageView>(mDevice, img.get(), imageViewDesc));
            mImages.push_back(std::move(img));
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
        mDevice.GetVulkanDevice().waitIdle();

        DestroyImageViews();
        DestroySemaphores();
        DestroyFences();
        CreateSwapchain(mWindow);
        CreateSemaphores();
        CreateFences();
        CreateImageViews();
    }
} // namespace Artus::Graphics
