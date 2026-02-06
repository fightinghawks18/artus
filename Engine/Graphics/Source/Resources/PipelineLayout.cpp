//
// Created by fightinghawks18 on 2/4/2026.
//

#include "Artus/Graphics/Resources/PipelineLayout.h"

namespace Artus::Graphics {
    PipelineLayout::PipelineLayout(Device& device) : mDevice(device) {
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.setSetLayouts(nullptr).setPushConstantRanges(nullptr);
        mPipelineLayout = device.GetVulkanDevice().createPipelineLayoutUnique(pipelineLayoutInfo);
    }

    PipelineLayout::~PipelineLayout() = default;
} // namespace Artus::Graphics