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

    inline RHI::Format FromVkFormat(const vk::Format format) {
        switch (format) {
        case vk::Format::eUndefined:
            return RHI::Format::Unknown;
        case vk::Format::eR8G8B8A8Sint:
            return RHI::Format::RGBA8_SInt;
        case vk::Format::eR8G8B8A8Srgb:
            return RHI::Format::RGBA8_SRgb;
        case vk::Format::eR8G8B8A8Unorm:
            return RHI::Format::RGBA8_UNorm;
        case vk::Format::eR8G8B8A8Snorm:
            return RHI::Format::RGBA8_SNorm;
        case vk::Format::eB8G8R8A8Sint:
            return RHI::Format::BGRA8_SInt;
        case vk::Format::eB8G8R8A8Srgb:
            return RHI::Format::BGRA8_SRgb;
        case vk::Format::eB8G8R8A8Unorm:
            return RHI::Format::BGRA8_UNorm;
        case vk::Format::eB8G8R8A8Snorm:
            return RHI::Format::BGRA8_SNorm;
        case vk::Format::eD16UnormS8Uint:
            return RHI::Format::D16_UNorm_S8_UInt;
        case vk::Format::eD32SfloatS8Uint:
            return RHI::Format::D32_SFloat_S8_UInt;
        default:
            return RHI::Format::Unknown;
        }
    }
}

#endif // ARTUS_VULKAN_FORMAT_H
