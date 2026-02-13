//
// Created by fightinghawks18 on 2/7/2026.
//

#include "Artus/Graphics/Vulkan/Resources/DescriptorAllocator.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    DescriptorAllocator::DescriptorAllocator(Device& device, const DescriptorAllocatorCreateDesc& desc) : mDevice(device) {
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
                          .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);

        mDescriptorPool = device.GetVulkanDevice().createDescriptorPoolUnique(descriptorPoolInfo);
    }

    DescriptorAllocator::~DescriptorAllocator() {
        mDescriptorSets.clear();
        mDescriptorSetLayouts.clear();
        mDescriptorPool.reset();
    }

    DescriptorSetLayout* DescriptorAllocator::CreateDescriptorSetLayout(
        const std::vector<DescriptorSetLayoutBinding>& bindings) {
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
