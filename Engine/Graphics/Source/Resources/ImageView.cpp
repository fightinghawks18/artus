//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Resources/ImageView.h"

namespace Artus::Graphics {
    ImageView::ImageView(Device& device, Image* image, const ImageViewDesc& desc) : mDevice(device) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(desc.aspectMask)
            .setBaseArrayLayer(desc.baseLayer)
            .setBaseMipLevel(desc.baseLevel)
            .setLayerCount(desc.layerCount)
            .setLevelCount(desc.levelCount);

        vk::ComponentMapping componentMapping = {};
        componentMapping.setR(desc.redComponent)
            .setG(desc.greenComponent)
            .setB(desc.blueComponent)
            .setA(desc.alphaComponent);

        vk::ImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.setImage(image->GetVulkanImage())
            .setViewType(desc.type)
            .setFormat(desc.format)
            .setSubresourceRange(subresourceRange)
            .setComponents(componentMapping);

        mImageView = device.GetVulkanDevice().createImageViewUnique(imageViewInfo);
    }

    ImageView::~ImageView() = default;
}