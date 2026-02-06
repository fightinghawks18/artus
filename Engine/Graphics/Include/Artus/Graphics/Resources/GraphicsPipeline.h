//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_GRAPHICS_PIPELINE_H
#define ARTUS_GRAPHICS_PIPELINE_H

#include "../Device.h"
#include "PipelineLayout.h"
#include "Shader.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    struct GraphicsPipelineInputAttribute {
        uint32_t location;
        vk::Format format;
        uint32_t offset;
    };

    struct GraphicsPipelineInputBinding {
        uint32_t slot;
        uint32_t stride;
        std::vector<GraphicsPipelineInputAttribute> attributes;
    };

    struct GraphicsPipelineDescriptor {
        Shader* vertexStage;
        Shader* pixelStage;

        std::vector<GraphicsPipelineInputBinding> bindings;
        std::vector<vk::Format> colorFormats;
        vk::Format depthFormat;
        vk::Format stencilFormat;
        PipelineLayout* pipelineLayout;
    };

    class GraphicsPipeline {
    public:
        explicit GraphicsPipeline(Device& device, const GraphicsPipelineDescriptor& pipelineDescriptor);
        ~GraphicsPipeline();

        [[nodiscard]] vk::Pipeline GetVulkanPipeline() { return mPipeline.get(); }

    private:
        Device& mDevice;

        vk::UniquePipeline mPipeline;
    };
} // namespace Artus::Graphics

#endif // ARTUS_GRAPHICS_PIPELINE_H