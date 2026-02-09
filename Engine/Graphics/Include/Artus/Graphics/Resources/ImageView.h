//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_IMAGE_VIEW_H
#define ARTUS_IMAGE_VIEW_H

#include "../Device.h"
#include "Image.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    struct ImageViewDesc {
        vk::Format format;
        vk::ImageViewType type;
        vk::ImageAspectFlags aspectMask;
        uint32_t baseLayer;
        uint32_t layerCount;
        uint32_t baseLevel;
        uint32_t levelCount;

        vk::ComponentSwizzle redComponent;
        vk::ComponentSwizzle greenComponent;
        vk::ComponentSwizzle blueComponent;
        vk::ComponentSwizzle alphaComponent;
    };

    class ImageView {
    public:
        explicit ImageView(Device& device, Image* image, const ImageViewDesc& desc);
        ~ImageView();

        [[nodiscard]] vk::ImageView GetVulkanImageView() const { return mImageView.get(); }

    private:
        Device& mDevice;

        vk::UniqueImageView mImageView;
    };
}

#endif // ARTUS_IMAGE_VIEW_H