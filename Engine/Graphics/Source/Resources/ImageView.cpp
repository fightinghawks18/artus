//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Resources/ImageView.h"

namespace Artus::Graphics {
    ImageView::ImageView(Device& device, Image* image, vk::Format imageFormat, vk::ImageViewType viewType, vk::ImageSubresourceRange range) : mDevice(device) {
        vk::ImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.setImage(image->GetVulkanImage()).setViewType(viewType).setFormat(imageFormat).setSubresourceRange(range);

        mImageView = device.GetVulkanDevice().createImageViewUnique(imageViewInfo);
    }

    ImageView::~ImageView() = default;
}