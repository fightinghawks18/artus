//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_IMAGE_VIEW_H
#define ARTUS_IMAGE_VIEW_H

#include "../Device.h"
#include "Image.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class ImageView {
    public:
        explicit ImageView(Device& device, Image* image, vk::Format imageFormat, vk::ImageViewType viewType, vk::ImageSubresourceRange range);
        ~ImageView();

        [[nodiscard]] vk::ImageView GetVulkanImageView() const { return mImageView.get(); }

    private:
        Device& mDevice;

        vk::UniqueImageView mImageView;
    };
}

#endif // ARTUS_IMAGE_VIEW_H