//
// Created by fightinghawks18 on 2/7/2026.
//

#ifndef ARTUS_DESCRIPTOR_SET_H
#define ARTUS_DESCRIPTOR_SET_H

#include "DescriptorSetLayout.h"
#include "Buffer.h"
#include "ImageView.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class DescriptorSet {
    public:
        explicit DescriptorSet(Device& device, vk::DescriptorPool pool, DescriptorSetLayout* layout);
        ~DescriptorSet();

        void WriteDescriptorSet(uint32_t binding, uint32_t offset, vk::DescriptorType type, const Buffer* buffer);
        void WriteDescriptorSet(uint32_t binding, uint32_t offset, vk::DescriptorType type, const ImageView* imageView,
                                vk::ImageLayout imageLayout);

        [[nodiscard]] vk::DescriptorSet GetVulkanDescriptorSet() { return mDescriptorSet.get(); }

    private:
        Device& mDevice;

        vk::UniqueDescriptorSet mDescriptorSet;
    };
}

#endif // ARTUS_DESCRIPTOR_SET_H
