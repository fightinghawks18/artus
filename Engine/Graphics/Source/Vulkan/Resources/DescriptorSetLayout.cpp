//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Resources/DescriptorSetLayout.h"

namespace Artus::Graphics {
    DescriptorSetLayout::DescriptorSetLayout(Device& device, const std::vector<DescriptorSetLayoutBinding>& bindings) : mDevice(device) {
        std::vector<vk::DescriptorSetLayoutBinding> vbindings;
        vbindings.reserve(bindings.size());
        for (const auto& binding : bindings) {
            vk::DescriptorSetLayoutBinding vbinding = {};
            vbinding.setBinding(binding.binding)
                .setDescriptorType(binding.type)
                .setImmutableSamplers(nullptr)
                .setStageFlags(binding.stageFlags);

            vbinding.descriptorCount = binding.count;

            vbindings.push_back(vbinding);
        }

        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
        descriptorSetLayoutInfo.setBindings(vbindings);

        mDescriptorSetLayout = device.GetVulkanDevice().createDescriptorSetLayoutUnique(descriptorSetLayoutInfo);
    }

    DescriptorSetLayout::~DescriptorSetLayout() = default;
}