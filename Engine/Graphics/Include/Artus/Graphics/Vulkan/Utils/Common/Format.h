//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_VULKAN_FORMAT_H
#define ARTUS_VULKAN_FORMAT_H

#include "Artus/Graphics/Enums.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    inline vk::Format ToVkFormat(const Enums::Format format) {
        switch (format) {
        case Enums::Format::Unknown:
            return vk::Format::eUndefined;
        case Enums::Format::RGBA8_SInt:
            return vk::Format::eR8G8B8A8Sint;
        case Enums::Format::RGBA8_SRgb:
            return vk::Format::eR8G8B8A8Srgb;
        case Enums::Format::RGBA8_UNorm:
            return vk::Format::eR8G8B8A8Unorm;
        case Enums::Format::RGBA8_SNorm:
            return vk::Format::eR8G8B8A8Snorm;
        case Enums::Format::BGRA8_SInt:
            return vk::Format::eB8G8R8A8Sint;
        case Enums::Format::BGRA8_SRgb:
            return vk::Format::eB8G8R8A8Srgb;
        case Enums::Format::BGRA8_UNorm:
            return vk::Format::eB8G8R8A8Unorm;
        case Enums::Format::BGRA8_SNorm:
            return vk::Format::eB8G8R8A8Snorm;
        case Enums::Format::RG32_Float:
            return vk::Format::eR32G32Sfloat;
        case Enums::Format::RGB32_Float:
            return vk::Format::eR32G32B32Sfloat;
        case Enums::Format::RGBA32_Float:
            return vk::Format::eR32G32B32A32Sfloat;
        case Enums::Format::D16_UNorm_S8_UInt:
            return vk::Format::eD16UnormS8Uint;
        case Enums::Format::D32_SFloat_S8_UInt:
            return vk::Format::eD32SfloatS8Uint;
        default:
            return vk::Format::eUndefined;
        }
    }

    inline Enums::Format FromVkFormat(const vk::Format format) {
        switch (format) {
        case vk::Format::eUndefined:
            return Enums::Format::Unknown;
        case vk::Format::eR8G8B8A8Sint:
            return Enums::Format::RGBA8_SInt;
        case vk::Format::eR8G8B8A8Srgb:
            return Enums::Format::RGBA8_SRgb;
        case vk::Format::eR8G8B8A8Unorm:
            return Enums::Format::RGBA8_UNorm;
        case vk::Format::eR8G8B8A8Snorm:
            return Enums::Format::RGBA8_SNorm;
        case vk::Format::eB8G8R8A8Sint:
            return Enums::Format::BGRA8_SInt;
        case vk::Format::eB8G8R8A8Srgb:
            return Enums::Format::BGRA8_SRgb;
        case vk::Format::eB8G8R8A8Unorm:
            return Enums::Format::BGRA8_UNorm;
        case vk::Format::eB8G8R8A8Snorm:
            return Enums::Format::BGRA8_SNorm;
        case vk::Format::eR32G32Sfloat:
            return Enums::Format::RG32_Float;
        case vk::Format::eR32G32B32Sfloat:
            return Enums::Format::RGB32_Float;
        case vk::Format::eR32G32B32A32Sfloat:
            return Enums::Format::RGBA32_Float;
        case vk::Format::eD16UnormS8Uint:
            return Enums::Format::D16_UNorm_S8_UInt;
        case vk::Format::eD32SfloatS8Uint:
            return Enums::Format::D32_SFloat_S8_UInt;
        default:
            return Enums::Format::Unknown;
        }
    }
}

#endif // ARTUS_VULKAN_FORMAT_H
