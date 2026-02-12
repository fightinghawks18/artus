//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_COMMON_H
#define ARTUS_VULKAN_COMMON_H

#include "Artus/Graphics/RHI/Types/Common.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    inline vk::Rect2D ToVkRect2D(const RHI::Rectangle& rect) {
        vk::Rect2D rect2D = {};
        rect2D.setOffset({rect.x, rect.y}).setExtent({rect.width, rect.height});
        return rect2D;
    }

    inline vk::Extent2D ToVkExtent2D(const RHI::Extent& extent) { return {extent.width, extent.height}; }

    inline vk::Extent3D ToVkExtent3D(const RHI::Extent3D& extent) {
        return {extent.width, extent.height, extent.depth};
    }
}

#endif // ARTUS_VULKAN_COMMON_H
