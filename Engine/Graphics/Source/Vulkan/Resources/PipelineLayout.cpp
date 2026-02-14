//
// Created by fightinghawks18 on 2/4/2026.
//

#include "Artus/Graphics/Vulkan/Resources/PipelineLayout.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    PipelineLayout::PipelineLayout(Device& device, const Structs::PipelineLayoutCreateDesc& desc) : mDevice(device) {
        std::vector<vk::DescriptorSetLayout> layouts;
        layouts.reserve(desc.groupLayouts.size());

        for (const auto& groupLayout : desc.groupLayouts) {
            const auto vkBindGroupLayout = reinterpret_cast<BindGroupLayout*>(groupLayout);
            layouts.push_back(vkBindGroupLayout->GetDescriptorSetLayout()->GetVulkanDescriptorSetLayout());
        }

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.setSetLayouts(layouts)
                          .setPushConstantRanges(nullptr); // WebGPU approach doesn't support push constants
        mPipelineLayout = device.GetVulkanDevice().createPipelineLayoutUnique(pipelineLayoutInfo);
    }

    PipelineLayout::~PipelineLayout() = default;
} // namespace Artus::Graphics
