//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_GRAPHICS_PIPELINE_H
#define ARTUS_GRAPHICS_PIPELINE_H

#include "../Device.h"
#include "PipelineLayout.h"
#include "Shader.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class GraphicsPipeline {
    public:
        explicit GraphicsPipeline(Device& device, const GraphicsPipelineDesc& pipelineDescriptor);
        ~GraphicsPipeline();

        [[nodiscard]] vk::Pipeline GetVulkanPipeline() { return mPipeline.get(); }

    private:
        Device& mDevice;

        vk::UniquePipeline mPipeline;
    };
} // namespace Artus::Graphics

#endif // ARTUS_GRAPHICS_PIPELINE_H