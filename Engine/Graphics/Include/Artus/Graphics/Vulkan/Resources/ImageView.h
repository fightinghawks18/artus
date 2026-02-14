//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_IMAGE_VIEW_H
#define ARTUS_IMAGE_VIEW_H

#include "Image.h"
#include "Artus/Graphics/Enums.h"
#include "Artus/Graphics/Structs.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::ImageAspectFlags ToVkImageAspectFlags(const Flags::ImageAspect imageAspect) {
        vk::ImageAspectFlags flags = vk::ImageAspectFlagBits::eNone;
        if ((imageAspect & Flags::ImageAspect::Color) != Flags::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eColor;
        if ((imageAspect & Flags::ImageAspect::Depth) != Flags::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eDepth;
        if ((imageAspect & Flags::ImageAspect::Stencil) != Flags::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eStencil;
        return flags;
    }

    inline vk::ImageViewType ToVkImageViewType(const Enums::ImageViewType imageViewType) {
        switch (imageViewType) {
        case Enums::ImageViewType::ImageView2D:
            return vk::ImageViewType::e2D;
        case Enums::ImageViewType::ImageView3D:
            return vk::ImageViewType::e3D;
        default:
            return vk::ImageViewType::e2D;
        }
    }

    class ImageView {
    public:
        explicit ImageView(Device& device, const Structs::ImageViewCreateDesc& desc);
        ~ImageView();

        [[nodiscard]] Image* GetImage() const { return mImage; }
        [[nodiscard]] vk::ImageView GetVulkanImageView() const { return mImageView.get(); }

    private:
        Device& mDevice;

        Image* mImage;
        vk::UniqueImageView mImageView;
    };
}

#endif // ARTUS_IMAGE_VIEW_H
