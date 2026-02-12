//
// Created by fightinghawks18 on 2/7/2026.
//

#ifndef ARTUS_DESCRIPTOR_SET_LAYOUT_H
#define ARTUS_DESCRIPTOR_SET_LAYOUT_H

#include "../Device.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    struct DescriptorSetLayoutBinding {
        uint32_t binding;
        vk::DescriptorType type;
        vk::ShaderStageFlags stageFlags;
        uint32_t count;
    };

    class DescriptorSetLayout {
    public:
        explicit DescriptorSetLayout(Device& device, const std::vector<DescriptorSetLayoutBinding>& bindings);
        ~DescriptorSetLayout();

        [[nodiscard]] vk::DescriptorSetLayout GetVulkanDescriptorSetLayout() { return mDescriptorSetLayout.get(); }

    private:
        Device& mDevice;

        vk::UniqueDescriptorSetLayout mDescriptorSetLayout;
    };
}

#endif // ARTUS_DESCRIPTOR_SET_LAYOUT_H
