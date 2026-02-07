//
// Created by fightinghawks18 on 2/4/2026.
//

#include "Artus/Graphics/Resources/PipelineLayout.h"

namespace Artus::Graphics {
    PipelineLayout::PipelineLayout(Device& device, DescriptorSetLayout* setLayout) : mDevice(device) {
        auto layout = setLayout->GetVulkanDescriptorSetLayout();

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.setSetLayouts(layout).setPushConstantRanges(nullptr);
        mPipelineLayout = device.GetVulkanDevice().createPipelineLayoutUnique(pipelineLayoutInfo);
    }

    PipelineLayout::~PipelineLayout() = default;
} // namespace Artus::Graphics