//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_IMAGE_VIEW_H
#define ARTUS_IMAGE_VIEW_H

#include "Artus/Graphics/RHI/Resources/IImageView.h"
#include "Image.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::ImageAspectFlags ToVkImageAspectFlags(const RHI::ImageAspect imageAspect) {
        vk::ImageAspectFlags flags = vk::ImageAspectFlagBits::eNone;
        if ((imageAspect & RHI::ImageAspect::Color) != RHI::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eColor;
        if ((imageAspect & RHI::ImageAspect::Depth) != RHI::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eDepth;
        if ((imageAspect & RHI::ImageAspect::Stencil) != RHI::ImageAspect::None)
            flags |= vk::ImageAspectFlagBits::eStencil;
        return flags;
    }

    inline vk::ImageViewType ToVkImageViewType(const RHI::ImageViewType imageViewType) {
        switch (imageViewType) {
        case RHI::ImageViewType::ImageView2D:
            return vk::ImageViewType::e2D;
        case RHI::ImageViewType::ImageView3D:
            return vk::ImageViewType::e3D;
        default:
            return vk::ImageViewType::e2D;
        }
    }

    class ImageView : public RHI::IImageView {
    public:
        explicit ImageView(Device& device, const RHI::ImageViewDesc& desc);
        ~ImageView() override;

        [[nodiscard]] vk::ImageView GetVulkanImageView() const { return mImageView.get(); }

    private:
        Device& mDevice;

        vk::UniqueImageView mImageView;
    };
}

#endif // ARTUS_IMAGE_VIEW_H
