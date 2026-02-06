//
// Created by fightinghawks18 on 2/1/2026.
//

#include "Artus/Graphics/Resources/CommandEncoder.h"
#include <Artus/Core/Logger.h>

namespace Artus::Graphics {
    CommandEncoder::CommandEncoder(Device& device, vk::CommandPool commandPool) : mDevice(device) {
        vk::CommandBufferAllocateInfo cmdBufferInfo = {};
        cmdBufferInfo.setCommandBufferCount(1).setCommandPool(commandPool);

        std::vector<vk::UniqueCommandBuffer> cmdBuffers =
            device.GetVulkanDevice().allocateCommandBuffersUnique(cmdBufferInfo);
        mCommandBuffer = std::move(cmdBuffers[0]);
    }

    CommandEncoder::~CommandEncoder() {
        mDevice.GetVulkanDevice().waitIdle();

        if (mInUse) {
            End();
        }

        mCommandBuffer.reset();
    }

    void CommandEncoder::Start() {
        if (mInUse) {
            AR_WARN("Attempted to start CommandEncoder but it already started!");
            return;
        }

        vk::CommandBufferBeginInfo beginInfo = {};
        beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        mCommandBuffer->begin(beginInfo);
        mInUse = true;
    }

    void CommandEncoder::End() {
        if (!mInUse) {
            AR_WARN("Attempted to end CommandEncoder but it already ended!");
            return;
        }
        mCommandBuffer->end();
        mInUse = false;
    }

    void CommandEncoder::Reset() { mCommandBuffer->reset(); }
    void CommandEncoder::StartRendering(vk::RenderingInfo renderingInfo) {
        mCommandBuffer->beginRendering(renderingInfo);
    }
    void CommandEncoder::EndRendering() { mCommandBuffer->endRendering(); }

    void CommandEncoder::BindGraphicsPipeline(GraphicsPipeline* graphicsPipeline) {
        auto pipelineHandle = graphicsPipeline->GetVulkanPipeline();
        mCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineHandle);
    }

    void CommandEncoder::BindVertexBuffer(Buffer* vertexBuffer) {
        auto vertexHandle = vertexBuffer->GetVulkanBuffer();
        mCommandBuffer->bindVertexBuffers(0, vertexHandle, {0});
    }

    void CommandEncoder::BindIndexBuffer(Buffer* indexBuffer) {
        auto indexHandle = indexBuffer->GetVulkanBuffer();
        mCommandBuffer->bindIndexBuffer(indexHandle, 0, vk::IndexType::eUint32);
    }

    void CommandEncoder::DrawIndexed(uint32_t indexCount, uint32_t firstIndex) {
        mCommandBuffer->drawIndexed(indexCount, 0, firstIndex, 0, 0);
    }

} // namespace Artus::Graphics
