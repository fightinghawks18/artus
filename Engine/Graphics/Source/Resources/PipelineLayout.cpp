//
// Created by fightinghawks18 on 2/4/2026.
//

#include "Artus/Graphics/Resources/PipelineLayout.h"

namespace Artus::Graphics {
    PipelineLayout::PipelineLayout(Device& device, const PipelineLayoutDesc& desc) : mDevice(device) {
        std::vector<vk::DescriptorSetLayout> layouts;
        layouts.reserve(desc.layouts.size());

        std::vector<vk::PushConstantRange> pushConstants;
        pushConstants.reserve(desc.pushConstants.size());

        for (const auto& layout : desc.layouts) {
            layouts.push_back(layout->GetVulkanDescriptorSetLayout());
        }

        for (const auto& pushConstant : desc.pushConstants) {
            vk::PushConstantRange pushConstantRange = {};
            pushConstantRange.setSize(pushConstant.size).setOffset(pushConstant.offset).setStageFlags(pushConstant.stageFlags);
            pushConstants.push_back(pushConstantRange);
        }

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.setSetLayouts(layouts).setPushConstantRanges(pushConstants);
        mPipelineLayout = device.GetVulkanDevice().createPipelineLayoutUnique(pipelineLayoutInfo);
    }

    PipelineLayout::~PipelineLayout() = default;
} // namespace Artus::Graphics