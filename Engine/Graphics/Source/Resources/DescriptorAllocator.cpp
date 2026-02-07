//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Resources/DescriptorAllocator.h"

namespace Artus::Graphics {
    DescriptorAllocator::DescriptorAllocator(Device& device, const DescriptorAllocatorDesc& desc) : mDevice(device) {
        std::vector<vk::DescriptorPoolSize> poolSizes;
        poolSizes.reserve(desc.pools.size());
        for (const auto& pool : desc.pools) {
            vk::DescriptorPoolSize poolSize = {};
            poolSize.setDescriptorCount(pool.descriptorCount).setType(pool.type);
            poolSizes.push_back(poolSize);
        }

        vk::DescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.setMaxSets(desc.maxDescriptorSets)
            .setPoolSizes(poolSizes)
            .setFlags(vk::DescriptorPoolCreateFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT));

        mDescriptorPool = device.GetVulkanDevice().createDescriptorPoolUnique(descriptorPoolInfo);
    }

    DescriptorAllocator::~DescriptorAllocator() {
        mDescriptorSets.clear();
        mDescriptorSetLayouts.clear();
        mDescriptorPool.reset();
    }

    DescriptorSetLayout* DescriptorAllocator::CreateDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& bindings) {
        auto setLayout = std::make_unique<DescriptorSetLayout>(mDevice, bindings);
        auto setLayoutPtr = setLayout.get();
        mDescriptorSetLayouts.push_back(std::move(setLayout));
        return setLayoutPtr;
    }

    DescriptorSet* DescriptorAllocator::CreateDescriptorSet(DescriptorSetLayout* layout) {
        auto set = std::make_unique<DescriptorSet>(mDevice, mDescriptorPool.get(), layout);
        auto setPtr = set.get();
        mDescriptorSets.push_back(std::move(set));
        return setPtr;
    }
} // namespace Artus::Graphics