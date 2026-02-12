//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_GRAPHICS_PIPELINE_H
#define ARTUS_GRAPHICS_PIPELINE_H

#include "PipelineLayout.h"
#include "Artus/Graphics/RHI/Resources/IGraphicsPipeline.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::CompareOp ToVkCompareOp(const RHI::DepthComparison compareOp) {
        switch (compareOp) {
        case RHI::DepthComparison::Never:
            return vk::CompareOp::eNever;
        case RHI::DepthComparison::Less:
            return vk::CompareOp::eLess;
        case RHI::DepthComparison::LessOrEqual:
            return vk::CompareOp::eLessOrEqual;
        case RHI::DepthComparison::Greater:
            return vk::CompareOp::eGreater;
        case RHI::DepthComparison::GreaterOrEqual:
            return vk::CompareOp::eGreaterOrEqual;
        case RHI::DepthComparison::Always:
            return vk::CompareOp::eAlways;
        default:
            return vk::CompareOp::eNever;
        }
    }

    class GraphicsPipeline : public RHI::IGraphicsPipeline {
    public:
        explicit GraphicsPipeline(Device& device, const RHI::GraphicsPipelineDesc& pipelineDescriptor);
        ~GraphicsPipeline() override;

        [[nodiscard]] vk::Pipeline GetVulkanPipeline() { return mPipeline.get(); }

    private:
        Device& mDevice;

        vk::UniquePipeline mPipeline;
    };
} // namespace Artus::Graphics

#endif // ARTUS_GRAPHICS_PIPELINE_H
