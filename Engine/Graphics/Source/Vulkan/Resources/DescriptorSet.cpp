//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Vulkan/Resources/DescriptorSet.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    DescriptorSet::DescriptorSet(Device& device, vk::DescriptorPool pool,
                                 DescriptorSetLayout* layout) : mDevice(device) {
        auto setLayout = layout->GetVulkanDescriptorSetLayout();
        auto layouts = {setLayout};

        vk::DescriptorSetAllocateInfo descriptorSetInfo = {};
        descriptorSetInfo.setDescriptorSetCount(1).setDescriptorPool(pool).setSetLayouts(layouts);

        auto sets = device.GetVulkanDevice().allocateDescriptorSetsUnique(descriptorSetInfo);
        mDescriptorSet = std::move(sets[0]);
    }

    DescriptorSet::~DescriptorSet() = default;

    void DescriptorSet::WriteDescriptorSet(const uint32_t binding, const uint32_t offset,
                                           const vk::DescriptorType type, const Buffer* buffer) {
        vk::DescriptorBufferInfo bufferInfo = {};
        bufferInfo.setBuffer(buffer->GetVulkanBuffer()).setOffset(0).setRange(vk::WholeSize);

        vk::WriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.setBufferInfo(bufferInfo)
                          .setDescriptorType(type)
                          .setDstSet(mDescriptorSet.get())
                          .setDstBinding(binding).setDstArrayElement(offset);

        mDevice.GetVulkanDevice().updateDescriptorSets(writeDescriptorSet, nullptr);
    }

    void DescriptorSet::WriteDescriptorSet(const uint32_t binding, const uint32_t offset,
                                           const vk::DescriptorType type, const ImageView* imageView,
                                           const vk::ImageLayout imageLayout) {
        vk::DescriptorImageInfo imageInfo = {};
        imageInfo.setImageView(imageView->GetVulkanImageView()).setImageLayout(imageLayout).setSampler(nullptr);

        vk::WriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.setImageInfo(imageInfo)
                          .setDescriptorType(type)
                          .setDstSet(mDescriptorSet.get())
                          .setDstBinding(binding).setDstArrayElement(offset);

        mDevice.GetVulkanDevice().updateDescriptorSets(writeDescriptorSet, nullptr);
    }
}
