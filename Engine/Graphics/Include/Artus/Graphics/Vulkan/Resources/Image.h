//
// Created by fightinghawks18 on 1/25/2026.
//

#ifndef ARTUS_IMAGE_H
#define ARTUS_IMAGE_H

#include "../Utils/Vulkan/Allocator.h"
#include "Artus/Graphics/Enums.h"
#include "Artus/Graphics/Flags.h"
#include "Artus/Graphics/Structs.h"

#include <vulkan/vulkan.hpp>


namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::ImageUsageFlags ToVkImageUsageFlags(const Flags::ImageUsage imageUsage) {
        vk::ImageUsageFlags flags;
        if ((imageUsage & Flags::ImageUsage::Color) != Flags::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eColorAttachment;
        if ((imageUsage & Flags::ImageUsage::DepthStencil) != Flags::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        if ((imageUsage & Flags::ImageUsage::Shader) != Flags::ImageUsage::None)
            flags |= vk::ImageUsageFlagBits::eSampled;
        return flags;
    }

    inline vk::ImageType ToVkImageType(const Enums::ImageType imageType) {
        switch (imageType) {
        case Enums::ImageType::Image2D:
            return vk::ImageType::e2D;
        case Enums::ImageType::Image3D:
            return vk::ImageType::e3D;
        default:
            return vk::ImageType::e2D;
        }
    }

    inline Enums::ImageState FromVkImageLayout(const vk::ImageLayout imageLayout) {
        switch (imageLayout) {
        case vk::ImageLayout::eUndefined:
            return Enums::ImageState::Unknown;
        case vk::ImageLayout::eColorAttachmentOptimal:
            return Enums::ImageState::Color;
        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            return Enums::ImageState::DepthStencil;
        case vk::ImageLayout::eShaderReadOnlyOptimal:
            return Enums::ImageState::ShaderAccess;
        case vk::ImageLayout::ePresentSrcKHR:
            return Enums::ImageState::Presentable;
        default:
            return Enums::ImageState::Unknown;
        }
    }

    inline vk::ImageLayout ToVkImageLayout(const Enums::ImageState imageState) {
        switch (imageState) {
        case Enums::ImageState::Unknown:
            return vk::ImageLayout::eUndefined;
        case Enums::ImageState::Color:
            return vk::ImageLayout::eColorAttachmentOptimal;
        case Enums::ImageState::DepthStencil:
            return vk::ImageLayout::eDepthStencilAttachmentOptimal;
        case Enums::ImageState::ShaderAccess:
            return vk::ImageLayout::eShaderReadOnlyOptimal;
        case Enums::ImageState::Presentable:
            return vk::ImageLayout::ePresentSrcKHR;
        default:
            return vk::ImageLayout::eUndefined;
        }
    }

    /// @brief Holds image data used for textures, rendering, or storage
    class Image {
    public:
        explicit Image(Device& device, const Structs::ImageCreateDesc& desc);
        explicit Image(Device& device, vk::Image image);
        ~Image();

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
