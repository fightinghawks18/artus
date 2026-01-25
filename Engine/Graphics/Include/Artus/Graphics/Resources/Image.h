//
// Created by fightinghawks18 on 1/25/26.
//

#ifndef ARTUS_IMAGE_H
#define ARTUS_IMAGE_H

#include "Artus/Graphics/Device.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class Image {
    public:
        explicit Image(Device& device, const vk::ImageCreateInfo& imageInfo);
        explicit Image(Device& device, vk::Image image);
        ~Image();

        void MakeRenderable(vk::CommandBuffer cmd);
        void MakeDepthStencil(vk::CommandBuffer cmd);
        void MakeShaderResource(vk::CommandBuffer cmd);
        void MakePresentable(vk::CommandBuffer cmd);

        [[nodiscard]] vk::Image GetVulkanImage() const { return mImage; }
    private:
        Device& mDevice;

        bool mAllocated;
        vk::Image mImage;
        vk::ImageLayout mCurrentState;
        vk::AccessFlags2 mAccessMasks;
        vk::PipelineStageFlags2 mStageMasks;
    };
}

#endif // ARTUS_IMAGE_H
