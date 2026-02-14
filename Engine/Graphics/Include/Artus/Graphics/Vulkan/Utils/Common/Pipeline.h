//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_COMMON_PIPELINE_H
#define ARTUS_VULKAN_COMMON_PIPELINE_H

#include "Artus/Graphics/Enums.h"

namespace Artus::Graphics::Vulkan {
    inline vk::ShaderStageFlags ToVkShaderStageFlags(const Enums::ShaderStage shaderStage) {
        switch (shaderStage) {
        case Enums::ShaderStage::Vertex:
            return vk::ShaderStageFlagBits::eVertex;
        case Enums::ShaderStage::Pixel:
            return vk::ShaderStageFlagBits::eFragment;
        case Enums::ShaderStage::Compute:
            return vk::ShaderStageFlagBits::eCompute;
        default:
            return vk::ShaderStageFlagBits::eVertex;
        }
    }

    inline vk::DescriptorType ToVkDescriptorType(const Enums::BindGroupLayoutBindingType type) {
        switch (type) {
        case Enums::BindGroupLayoutBindingType::Buffer:
            return vk::DescriptorType::eUniformBuffer;
        case Enums::BindGroupLayoutBindingType::ImageView:
            return vk::DescriptorType::eSampledImage;
        default:
            return vk::DescriptorType::eUniformBuffer;
        }
    }
}

#endif // ARTUS_VULKAN_COMMON_PIPELINE_H
