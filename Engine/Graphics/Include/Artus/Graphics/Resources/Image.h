//
// Created by fightinghawks18 on 1/25/2026.
//

#ifndef ARTUS_IMAGE_H
#define ARTUS_IMAGE_H

#include "Artus/Graphics/Device.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    struct ImageDesc {
        vk::Format format;
        vk::ImageType type;
        vk::Extent3D extent;
        vk::ImageUsageFlags usage;
        uint32_t layerCount;
        uint32_t levelCount;
    };

    /// @brief Holds image data used for textures, rendering, or storage
    class Image {
    public:
        explicit Image(Device& device, const ImageDesc& desc);
        explicit Image(Device& device, vk::Image image);
        ~Image();

        /// @brief Modifies this image to be available for use in rendering
        /// @param cmd The command buffer that modifies this image's state
        void MakeRenderable(vk::CommandBuffer cmd);
        /// @brief Modifies this image to be available for use in depth stenciling
        /// @param cmd The command buffer that modifies this image's state
        void MakeDepthStencil(vk::CommandBuffer cmd);
        /// @brief Modifies this image to be available for use in shaders
        /// @param cmd The command buffer that modifies this image's state
        void MakeShaderResource(vk::CommandBuffer cmd);
        /// @brief Modifies this image to be available for presenting to the screen
        /// @param cmd The command buffer that modifies this image's state
        void MakePresentable(vk::CommandBuffer cmd);

        /// @brief Retrieves the vulkan handle to this image
        /// @return vk::Image
        [[nodiscard]] vk::Image GetVulkanImage() const { return mImage; }

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
