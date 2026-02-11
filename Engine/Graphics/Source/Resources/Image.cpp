//
// Created by fightinghawks18 on 1/25/2026.
//

#include "Artus/Graphics/Resources/Image.h"

namespace Artus::Graphics {
    Image::Image(Device& device, const ImageDesc& desc) : mDevice(device) {
        vk::ImageCreateInfo imageInfo = {};
        imageInfo.setFormat(desc.format)
            .setImageType(desc.type)
            .setMipLevels(desc.levelCount)
            .setArrayLayers(desc.layerCount)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setExtent(desc.extent)
            .setUsage(desc.usage)
            .setTiling(vk::ImageTiling::eOptimal)
            .setSharingMode(vk::SharingMode::eExclusive)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setQueueFamilyIndices(nullptr);

        VmaAllocationCreateInfo allocationInfo = {0};

        VkImage image;
        vmaCreateImage(device.GetVulkanAllocator(), imageInfo, &allocationInfo, &image, &mAllocation, nullptr);

        mImage = image;
        mCurrentState = imageInfo.initialLayout;
        mAccessMasks = vk::AccessFlagBits2::eNone;
        mStageMasks = vk::PipelineStageFlagBits2::eNone;
    }

    Image::Image(Device& device, const vk::Image image) : mDevice(device) {
        mImage = image;
        mAllocation = nullptr;
        mCurrentState = vk::ImageLayout::eUndefined;
        mAccessMasks = vk::AccessFlagBits2::eNone;
        mStageMasks = vk::PipelineStageFlagBits2::eNone;
    }

    Image::~Image() {
        mDevice.GetVulkanDevice().waitIdle();

        if (!mAllocation)
            return;
        vmaDestroyImage(mDevice.GetVulkanAllocator(), mImage, mAllocation);
    }

} // namespace Artus::Graphics