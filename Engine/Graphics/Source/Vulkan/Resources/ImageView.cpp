//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Vulkan/Resources/ImageView.h"
#include "Artus/Graphics/Vulkan/Utils/Common/Format.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    ImageView::ImageView(Device& device, const Structs::ImageViewCreateDesc& desc) : mDevice(device), mImage(desc.image) {
        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(ToVkImageAspectFlags(desc.aspectMask))
                        .setBaseArrayLayer(desc.baseLayer)
                        .setBaseMipLevel(desc.baseLevel)
                        .setLayerCount(desc.layerCount)
                        .setLevelCount(desc.levelCount);

        vk::ComponentMapping componentMapping = {};
        componentMapping.setR(vk::ComponentSwizzle::eIdentity)
                        .setG(vk::ComponentSwizzle::eIdentity)
                        .setB(vk::ComponentSwizzle::eIdentity)
                        .setA(vk::ComponentSwizzle::eIdentity);

        const auto vkImage = reinterpret_cast<Image*>(desc.image);

        vk::ImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.setImage(vkImage->GetVulkanImage())
                     .setViewType(ToVkImageViewType(desc.type))
                     .setFormat(ToVkFormat(desc.format))
                     .setSubresourceRange(subresourceRange)
                     .setComponents(componentMapping);

        mImageView = device.GetVulkanDevice().createImageViewUnique(imageViewInfo);
    }

    ImageView::~ImageView() = default;
}
