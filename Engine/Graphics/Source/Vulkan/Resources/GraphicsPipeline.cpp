//
// Created by fightinghawks18 on 2/2/2026.
//

#include "Artus/Graphics/Vulkan/Resources/GraphicsPipeline.h"
#include "Artus/Graphics/Vulkan/Utils/Common/Format.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include "Artus/Core/Logger.h"
#include "Artus/Graphics/Vulkan/Resources/Shader.h"

namespace Artus::Graphics::Vulkan {
    GraphicsPipeline::GraphicsPipeline(Device& device, const RHI::GraphicsPipelineCreateDesc& pipelineDescriptor)
        : mDevice(device) {
        const auto vkVertexShader = reinterpret_cast<Shader*>(pipelineDescriptor.vertexStage);
        const auto vkPixelShader = reinterpret_cast<Shader*>(pipelineDescriptor.pixelStage);

        vk::PipelineShaderStageCreateInfo vertexStageInfo = {};
        vertexStageInfo.setStage(vk::ShaderStageFlagBits::eVertex)
                       .setModule(vkVertexShader->GetVulkanShaderModule())
                       .setPName("main");
        vk::PipelineShaderStageCreateInfo pixelStageInfo = {};
        pixelStageInfo.setStage(vk::ShaderStageFlagBits::eFragment)
                      .setModule(vkPixelShader->GetVulkanShaderModule())
                      .setPName("main");

        auto stages = {vertexStageInfo, pixelStageInfo};

        std::vector<vk::VertexInputBindingDescription> bindings;
        std::vector<vk::VertexInputAttributeDescription> attributes;
        for (const auto& binding : pipelineDescriptor.bindings) {
            vk::VertexInputBindingDescription bind = {};
            bind.setBinding(binding.slot).setStride(binding.stride).setInputRate(vk::VertexInputRate::eVertex);
            bindings.push_back(bind);

            for (const auto& attribute : binding.attributes) {
                vk::VertexInputAttributeDescription attr = {};
                attr.setBinding(binding.slot)
                    .setFormat(ToVkFormat(attribute.format))
                    .setLocation(attribute.location)
                    .setOffset(attribute.offset);

                attributes.push_back(attr);
            }
        }

        vk::PipelineVertexInputStateCreateInfo vertexInputState = {};
        vertexInputState.setVertexAttributeDescriptions(attributes).setVertexBindingDescriptions(bindings);

        // Force dynamic states?
        static std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eDepthTestEnable, vk::DynamicState::eDepthWriteEnable,
            vk::DynamicState::eStencilTestEnable, vk::DynamicState::eCullMode,
            vk::DynamicState::eScissorWithCount, vk::DynamicState::eViewportWithCount,
            vk::DynamicState::eDepthBounds, vk::DynamicState::eDepthBoundsTestEnable
        };

        vk::PipelineDynamicStateCreateInfo dynamicStateInfo = {};
        dynamicStateInfo.setDynamicStates(dynamicStates);

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.setTopology(vk::PrimitiveTopology::eTriangleList).setPrimitiveRestartEnable(false);

        vk::PipelineTessellationStateCreateInfo tessellationInfo = {};
        tessellationInfo.setPatchControlPoints(4); // Use bilinear for now

        vk::PipelineViewportStateCreateInfo viewportStateInfo = {};
        viewportStateInfo.setViewports(nullptr).setScissors(
            nullptr); // Despite being dynamic, it needs to be provided in the pipeline info

        vk::PipelineRasterizationStateCreateInfo rasterizationStateInfo = {};
        rasterizationStateInfo.setCullMode(vk::CullModeFlagBits::eBack)
                              .setPolygonMode(vk::PolygonMode::eFill)
                              .setFrontFace(vk::FrontFace::eCounterClockwise)
                              .setDepthBiasEnable(false) // Uses offset to avoid z-fighting
                              .setDepthClampEnable(false)
                              // Forces(?) fragments to be within the near/far ranges of the viewport
                              .setDepthBiasClamp(0.0)
                              // Max offset that can be used to avoid z-fighting, 0.0 for no limit
                              .setDepthBiasConstantFactor(0.01) // Constant offset
                              .setDepthBiasSlopeFactor(0.02)
                              // Offset based on the angle that this primitive is viewed at
                              .setLineWidth(1.0f);

        vk::PipelineMultisampleStateCreateInfo multisampleStateInfo = {};
        multisampleStateInfo
            .setAlphaToCoverageEnable(true) // Use MSAA samples to smooth alpha transitions
            .setAlphaToOneEnable(
                false) // Enables whether all MSAA samples' alpha values are clamped to 1.0 to avoid double transparency
            .setSampleShadingEnable(
                false) // Enables whether the fragment shader runs for every sample rather than once per pixel
            .setRasterizationSamples(vk::SampleCountFlagBits::e1) // 1 MSAA sample
            .setPSampleMask(nullptr);

        vk::StencilOpState backFaceStencilOpStateInfo = {};
        backFaceStencilOpStateInfo.setCompareOp(vk::CompareOp::eAlways)
                                  .setDepthFailOp(vk::StencilOp::eKeep)
                                  // What todo if the pixel being stenciled is obscured
                                  .setFailOp(vk::StencilOp::eKeep)
                                  // eKeep is no-op, fragments will still be discarded if failed, but stencil
                                  // values can still be modified if something like eIncrementAndWrap was
                                  // chosen as the fail operation
                                  .setPassOp(vk::StencilOp::eIncrementAndWrap);

        vk::StencilOpState frontFaceStencilOpStateInfo = {};
        frontFaceStencilOpStateInfo.setCompareOp(vk::CompareOp::eAlways)
                                   .setDepthFailOp(vk::StencilOp::eKeep)
                                   .setFailOp(vk::StencilOp::eKeep)
                                   .setPassOp(vk::StencilOp::eDecrementAndWrap);

        vk::PipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
        depthStencilStateInfo.setDepthBoundsTestEnable(true)
                             .setDepthTestEnable(true) // Ensures farther objects don't render over this primitive
                             .setStencilTestEnable(true)
                             // Ensures that unmarked pixels of this primitive aren't rendered
                             .setDepthWriteEnable(true) // Writes depth values for the depth buffer
                             .setDepthCompareOp(ToVkCompareOp(pipelineDescriptor.depthCompare))
                             // Less is common (0.0 is closest, 1.0 is farthest)
                             .setBack(backFaceStencilOpStateInfo)
                             .setFront(frontFaceStencilOpStateInfo);

        vk::PipelineColorBlendAttachmentState attachmentStateInfo = {};
        attachmentStateInfo.setColorBlendOp(vk::BlendOp::eAdd)
                           .setDstColorBlendFactor(vk::BlendFactor::eDstColor)
                           .setSrcColorBlendFactor(vk::BlendFactor::eSrcColor)
                           .setDstAlphaBlendFactor(vk::BlendFactor::eDstAlpha)
                           .setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
                           .setBlendEnable(false) // Do not blend with the screen contents
                           .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                               vk::ColorComponentFlagBits::eB |
                               vk::ColorComponentFlagBits::eA); // Write to all color channels

        vk::PipelineColorBlendStateCreateInfo colorBlendStateInfo = {};
        colorBlendStateInfo.setBlendConstants({1.0f, 1.0f, 1.0f, 1.0f})
                           .setAttachments(attachmentStateInfo)
                           .setLogicOpEnable(false);
        // Toggles whether logic math is used on the color data instead of mixing

        std::vector<vk::Format> vkColorFormats = {};
        for (const auto& colorFormat : pipelineDescriptor.colorFormats) {
            vkColorFormats.push_back(ToVkFormat(colorFormat));
        }

        vk::PipelineRenderingCreateInfo pipelineRenderingInfo = {};
        pipelineRenderingInfo.setColorAttachmentFormats(vkColorFormats)
                             .setDepthAttachmentFormat(ToVkFormat(pipelineDescriptor.depthFormat))
                             .setStencilAttachmentFormat(ToVkFormat(pipelineDescriptor.stencilFormat));

        const auto vkPipelineLayout = reinterpret_cast<PipelineLayout*>(pipelineDescriptor.pipelineLayout);

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.setPNext(pipelineRenderingInfo)
                    .setStages(stages)
                    .setPVertexInputState(&vertexInputState)
                    .setPInputAssemblyState(&inputAssemblyInfo)
                    .setPDynamicState(&dynamicStateInfo)
                    .setPTessellationState(&tessellationInfo)
                    .setPViewportState(&viewportStateInfo)
                    .setPRasterizationState(&rasterizationStateInfo)
                    .setPMultisampleState(&multisampleStateInfo)
                    .setPDepthStencilState(&depthStencilStateInfo)
                    .setPColorBlendState(&colorBlendStateInfo)
                    .setRenderPass(nullptr)
                    .setLayout(vkPipelineLayout->GetVulkanPipelineLayout());

        auto result = device.GetVulkanDevice().createGraphicsPipelineUnique(nullptr, pipelineInfo);
        if (result.result != vk::Result::eSuccess) {
            AR_ERR("Failed to create graphics pipeline!");
            return;
        }

        mPipeline = std::move(result.value);
    }

    GraphicsPipeline::~GraphicsPipeline() = default;
} // namespace Artus::Graphics
