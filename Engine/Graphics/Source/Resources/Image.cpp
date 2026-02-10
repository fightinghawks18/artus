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

    void Image::MakeRenderable(vk::CommandBuffer cmd) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
            .setBaseArrayLayer(0)
            .setLayerCount(1)
            .setBaseMipLevel(0)
            .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(mImage)
            .setOldLayout(mCurrentState)
            .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setSrcAccessMask(mAccessMasks)
            .setSrcStageMask(mStageMasks)
            .setDstAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
            .setDstStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
            .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        cmd.pipelineBarrier2(dependencyInfo);

        mCurrentState = vk::ImageLayout::eColorAttachmentOptimal;
        mAccessMasks = vk::AccessFlagBits2::eColorAttachmentWrite;
        mStageMasks = vk::PipelineStageFlagBits2::eColorAttachmentOutput;
    }

    void Image::MakeDepthStencil(vk::CommandBuffer cmd) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil)
            .setBaseArrayLayer(0)
            .setLayerCount(1)
            .setBaseMipLevel(0)
            .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(mImage)
            .setOldLayout(mCurrentState)
            .setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setSrcAccessMask(mAccessMasks)
            .setSrcStageMask(mStageMasks)
            .setDstAccessMask(vk::AccessFlagBits2::eDepthStencilAttachmentRead |
                              vk::AccessFlagBits2::eDepthStencilAttachmentWrite)
            .setDstStageMask(vk::PipelineStageFlagBits2::eLateFragmentTests |
                             vk::PipelineStageFlagBits2::eEarlyFragmentTests)
            .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        cmd.pipelineBarrier2(dependencyInfo);

        mCurrentState = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        mAccessMasks =
            vk::AccessFlagBits2::eDepthStencilAttachmentRead | vk::AccessFlagBits2::eDepthStencilAttachmentWrite;
        mStageMasks = vk::PipelineStageFlagBits2::eLateFragmentTests | vk::PipelineStageFlagBits2::eEarlyFragmentTests;
    }

    void Image::MakeShaderResource(vk::CommandBuffer cmd) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
            .setBaseArrayLayer(0)
            .setLayerCount(1)
            .setBaseMipLevel(0)
            .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(mImage)
            .setOldLayout(mCurrentState)
            .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
            .setSrcAccessMask(mAccessMasks)
            .setSrcStageMask(mStageMasks)
            .setDstAccessMask(vk::AccessFlagBits2::eShaderRead)
            .setDstStageMask(vk::PipelineStageFlagBits2::eVertexShader | vk::PipelineStageFlagBits2::eFragmentShader)
            .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        cmd.pipelineBarrier2(dependencyInfo);

        mCurrentState = vk::ImageLayout::eShaderReadOnlyOptimal;
        mAccessMasks = vk::AccessFlagBits2::eShaderRead;
        mStageMasks = vk::PipelineStageFlagBits2::eVertexShader | vk::PipelineStageFlagBits2::eFragmentShader;
    }

    void Image::MakePresentable(vk::CommandBuffer cmd) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
            .setBaseArrayLayer(0)
            .setLayerCount(1)
            .setBaseMipLevel(0)
            .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(mImage)
            .setOldLayout(mCurrentState)
            .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
            .setSrcAccessMask(mAccessMasks)
            .setSrcStageMask(mStageMasks)
            .setDstAccessMask(vk::AccessFlagBits2::eNone)
            .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
            .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        cmd.pipelineBarrier2(dependencyInfo);

        mCurrentState = vk::ImageLayout::ePresentSrcKHR;
        mAccessMasks = vk::AccessFlagBits2::eNone;
        mStageMasks = vk::PipelineStageFlagBits2::eNone;
    }
} // namespace Artus::Graphics