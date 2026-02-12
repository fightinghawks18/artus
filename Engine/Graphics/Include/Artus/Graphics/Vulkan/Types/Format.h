//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_VULKAN_FORMAT_H
#define ARTUS_VULKAN_FORMAT_H

#include "Artus/Graphics/RHI/Types/Format.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    inline vk::Format ToVkFormat(const RHI::Format format) {
        switch (format) {
        case RHI::Format::Unknown:
            return vk::Format::eUndefined;
        case RHI::Format::RGBA8_SInt:
            return vk::Format::eR8G8B8A8Sint;
        case RHI::Format::RGBA8_SRgb:
            return vk::Format::eR8G8B8A8Srgb;
        case RHI::Format::RGBA8_UNorm:
            return vk::Format::eR8G8B8A8Unorm;
        case RHI::Format::RGBA8_SNorm:
            return vk::Format::eR8G8B8A8Snorm;
        case RHI::Format::BGRA8_SInt:
            return vk::Format::eB8G8R8A8Sint;
        case RHI::Format::BGRA8_SRgb:
            return vk::Format::eB8G8R8A8Srgb;
        case RHI::Format::BGRA8_UNorm:
            return vk::Format::eB8G8R8A8Unorm;
        case RHI::Format::BGRA8_SNorm:
            return vk::Format::eB8G8R8A8Snorm;
        case RHI::Format::D16_UNorm_S8_UInt:
            return vk::Format::eD16UnormS8Uint;
        case RHI::Format::D32_SFloat_S8_UInt:
            return vk::Format::eD32SfloatS8Uint;
        default:
            return vk::Format::eUndefined;
        }
    }
}

#endif // ARTUS_VULKAN_FORMAT_H