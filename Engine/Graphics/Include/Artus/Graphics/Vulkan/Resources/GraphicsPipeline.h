//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_GRAPHICS_PIPELINE_H
#define ARTUS_GRAPHICS_PIPELINE_H

#include "PipelineLayout.h"
#include "Artus/Graphics/Enums.h"
#include "Artus/Graphics/Structs.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    inline vk::CompareOp ToVkCompareOp(const Enums::DepthComparison compareOp) {
        switch (compareOp) {
        case Enums::DepthComparison::Never:
            return vk::CompareOp::eNever;
        case Enums::DepthComparison::Less:
            return vk::CompareOp::eLess;
        case Enums::DepthComparison::LessOrEqual:
            return vk::CompareOp::eLessOrEqual;
        case Enums::DepthComparison::Greater:
            return vk::CompareOp::eGreater;
        case Enums::DepthComparison::GreaterOrEqual:
            return vk::CompareOp::eGreaterOrEqual;
        case Enums::DepthComparison::Always:
            return vk::CompareOp::eAlways;
        default:
            return vk::CompareOp::eNever;
        }
    }

    inline vk::CullModeFlags ToVkCullMode(const Enums::CullMode cullMode) {
        switch (cullMode) {
        case Enums::CullMode::Never:
            return vk::CullModeFlagBits::eNone;
        case Enums::CullMode::Back:
            return vk::CullModeFlagBits::eBack;
        case Enums::CullMode::Front:
            return vk::CullModeFlagBits::eFront;
        case Enums::CullMode::BackAndFront:
            return vk::CullModeFlagBits::eFrontAndBack;
        default:
            return vk::CullModeFlagBits::eNone;
        }
    }

    class GraphicsPipeline {
    public:
        explicit GraphicsPipeline(Device& device, const Structs::GraphicsPipelineCreateDesc& pipelineDescriptor);
        ~GraphicsPipeline();

        [[nodiscard]] vk::Pipeline GetVulkanPipeline() { return mPipeline.get(); }

    private:
        Device& mDevice;

        vk::UniquePipeline mPipeline;
    };
} // namespace Artus::Graphics

#endif // ARTUS_GRAPHICS_PIPELINE_H
