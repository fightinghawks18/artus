//
// Created by fightinghawks18 on 1/25/2026.
//

#ifndef ARTUS_IMAGE_H
#define ARTUS_IMAGE_H

#include "Artus/Graphics/Device.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    /// @brief Holds image data used for textures, rendering, or storage
    class Image {
    public:
        explicit Image(Device& device, const ImageDesc& desc);
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
