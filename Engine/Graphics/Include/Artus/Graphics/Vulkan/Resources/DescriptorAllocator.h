//
// Created by fightinghawks18 on 2/7/2026.
//

#ifndef ARTUS_DESCRIPTOR_ALLOCATOR_H
#define ARTUS_DESCRIPTOR_ALLOCATOR_H

#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    struct DescriptorAllocatorPoolCreateDesc {
        vk::DescriptorType type;
        uint32_t descriptorCount;
    };

    struct DescriptorAllocatorCreateDesc {
        uint32_t maxDescriptorSets;
        const std::vector<DescriptorAllocatorPoolCreateDesc>& pools;
    };

    class DescriptorAllocator {
    public:
        explicit DescriptorAllocator(Device& device, const DescriptorAllocatorCreateDesc& desc);
        ~DescriptorAllocator();

        DescriptorSetLayout* CreateDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& bindings);
        DescriptorSet* CreateDescriptorSet(DescriptorSetLayout* layout);

        [[nodiscard]] vk::DescriptorPool GetVulkanDescriptorPool() const { return mDescriptorPool.get(); }

        [[nodiscard]] DescriptorSetLayout* GetDescriptorSetLayout(uint32_t index) const {
            return mDescriptorSetLayouts[index].get();
        }

        [[nodiscard]] DescriptorSet* GetDescriptorSet(uint32_t index) const { return mDescriptorSets[index].get(); }

    private:
        Device& mDevice;

        vk::UniqueDescriptorPool mDescriptorPool;
        std::vector<std::unique_ptr<DescriptorSetLayout>> mDescriptorSetLayouts;
        std::vector<std::unique_ptr<DescriptorSet>> mDescriptorSets;
    };
}

#endif // ARTUS_DESCRIPTOR_ALLOCATOR_H
