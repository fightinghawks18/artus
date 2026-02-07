//
// Created by fightinghawks18 on 2/4/2026.
//

#ifndef ARTUS_PIPELINE_LAYOUT_H
#define ARTUS_PIPELINE_LAYOUT_H

#include "DescriptorSetLayout.h"
#include "../Device.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    struct PipelinePushConstant {
        uint32_t size;
        uint32_t offset;
        vk::ShaderStageFlags stageFlags;
    };

    struct PipelineLayoutDesc {
        std::vector<DescriptorSetLayout*> layouts;
        std::vector<PipelinePushConstant> pushConstants;
    };

    class PipelineLayout {
    public:
        explicit PipelineLayout(Device& device, const PipelineLayoutDesc& desc);
        ~PipelineLayout();

        [[nodiscard]] vk::PipelineLayout GetVulkanPipelineLayout() { return mPipelineLayout.get(); }

    private:
        Device& mDevice;

        vk::UniquePipelineLayout mPipelineLayout;
    };
} // namespace Artus::Graphics

#endif // ARTUS_PIPELINE_LAYOUT_H