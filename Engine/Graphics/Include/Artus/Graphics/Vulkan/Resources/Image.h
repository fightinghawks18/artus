//
// Created by fightinghawks18 on 1/25/2026.
//

#ifndef ARTUS_IMAGE_H
#define ARTUS_IMAGE_H

#include "Artus/Graphics/Vulkan/Utils/Vulkan/Allocator.h"
#include "Artus/Graphics/RHI/Resources/IImage.h"
#include <vulkan/vulkan.hpp>


namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::ImageUsageFlags ToVkImageUsageFlags(const RHI::ImageUsage imageUsage) {
        vk::ImageUsageFlags flags;
        if ((imageUsage & RHI::ImageUsage::Color) != RHI::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eColorAttachment;
        if ((imageUsage & RHI::ImageUsage::DepthStencil) != RHI::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        if ((imageUsage & RHI::ImageUsage::Shader) != RHI::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eSampled;
        return flags;
    }

    inline vk::ImageType ToVkImageType(const RHI::ImageType imageType) {
        switch (imageType) {
        case RHI::ImageType::Image2D:
            return vk::ImageType::e2D;
        case RHI::ImageType::Image3D:
            return vk::ImageType::e3D;
        default:
            return vk::ImageType::e2D;
        }
    }

    inline RHI::ImageState FromVkImageLayout(const vk::ImageLayout imageLayout) {
        switch (imageLayout) {
        case vk::ImageLayout::eUndefined:
            return RHI::ImageState::Unknown;
        case vk::ImageLayout::eColorAttachmentOptimal:
            return RHI::ImageState::Color;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            return RHI::ImageState::DepthStencil;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            return RHI::ImageState::ShaderAccess;
        case vk::ImageLayout::ePresentSrcKHR:
            return RHI::ImageState::Presentable;
        default:
            return RHI::ImageState::Unknown;
        }
    }

    inline vk::ImageLayout ToVkImageLayout(const RHI::ImageState imageState) {
        switch (imageState) {
        case RHI::ImageState::Unknown:
            return vk::ImageLayout::eUndefined;
        case RHI::ImageState::Color:
            return vk::ImageLayout::eColorAttachmentOptimal;
        case RHI::ImageState::DepthStencil:
            return vk::ImageLayout::eDepthStencilAttachmentOptimal;
        case RHI::ImageState::ShaderAccess:
            return vk::ImageLayout::eShaderReadOnlyOptimal;
        case RHI::ImageState::Presentable:
            return vk::ImageLayout::ePresentSrcKHR;
        default:
            return vk::ImageLayout::eUndefined;
        }
    }

    /// @brief Holds image data used for textures, rendering, or storage
    class Image : public RHI::IImage {
    public:
        explicit Image(Device& device, const RHI::ImageCreateDesc& desc);
        explicit Image(Device& device, vk::Image image);
        ~Image() override;

        /// @brief Retrieves the vulkan handle to this image
        /// @return vk::Image
        [[nodiscard]] vk::Image GetVulkanImage() const { return mImage; }

        [[nodiscard]] vk::ImageLayout& GetVulkanLayout() { return mCurrentState; }
        [[nodiscard]] vk::AccessFlags2& GetVulkanAccessMasks() { return mAccessMasks; }
        [[nodiscard]] vk::PipelineStageFlags2& GetVulkanStageMasks() { return mStageMasks; }

    private:
        Device& mDevice;

        VmaAllocation mAllocation;
        vk::Image mImage;
        vk::ImageLayout mCurrentState;
        vk::AccessFlags2 mAccessMasks;
        vk::PipelineStageFlags2 mStageMasks;
    };
} // namespace Artus::Graphics

#endif // ARTUS_IMAGE_H
