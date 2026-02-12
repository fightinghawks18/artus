//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_COMMON_PIPELINE_H
#define ARTUS_VULKAN_COMMON_PIPELINE_H
#include "Artus/Graphics/Vulkan/Resources/Shader.h"

namespace Artus::Graphics::Vulkan {
    inline vk::ShaderStageFlags ToVkShaderStageFlags(const RHI::ShaderStage shaderStage) {
        switch (shaderStage) {
        case RHI::ShaderStage::Vertex:
            return vk::ShaderStageFlagBits::eVertex;
        case RHI::ShaderStage::Pixel:
            return vk::ShaderStageFlagBits::eFragment;
        case RHI::ShaderStage::Compute:
            return vk::ShaderStageFlagBits::eCompute;
        default:
            return vk::ShaderStageFlagBits::eVertex;
        }
    }

    inline vk::DescriptorType ToVkDescriptorType(const RHI::BindGroupLayoutBindingType type) {
        switch (type) {
        case RHI::BindGroupLayoutBindingType::Buffer:
            return vk::DescriptorType::eUniformBuffer;
        case RHI::BindGroupLayoutBindingType::ImageView:
            return vk::DescriptorType::eSampledImage;
        default:
            return vk::DescriptorType::eUniformBuffer;
        }
    }
}

#endif // ARTUS_VULKAN_COMMON_PIPELINE_H
