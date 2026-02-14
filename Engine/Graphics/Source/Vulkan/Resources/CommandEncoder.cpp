//
// Created by fightinghawks18 on 2/1/2026.
//

#include "Artus/Graphics/Vulkan/Resources/CommandEncoder.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include <Artus/Core/Logger.h>

namespace Artus::Graphics::Vulkan {
    CommandEncoder::CommandEncoder(Device& device, vk::CommandPool commandPool) : mDevice(device) {
        vk::CommandBufferAllocateInfo cmdBufferInfo = {};
        cmdBufferInfo.setCommandBufferCount(1).setCommandPool(commandPool);

        std::vector<vk::UniqueCommandBuffer> cmdBuffers =
            device.GetVulkanDevice().allocateCommandBuffersUnique(cmdBufferInfo);
        mCommandBuffer = std::move(cmdBuffers[0]);
    }

    CommandEncoder::~CommandEncoder() {
        mDevice.GetVulkanDevice().waitIdle();

        if (mInUse) { CommandEncoder::End(); }

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

    void CommandEncoder::StartRenderingPass(const Structs::RenderingPass& renderingPass) {
        vk::RenderingInfo renderingInfo = {};

        std::vector<vk::RenderingAttachmentInfo> colorAttachments;
        vk::RenderingAttachmentInfo depthStencilAttachmentInfo = {};
        for (const auto& attachment : renderingPass.attachments) {
            const auto vkImageView = reinterpret_cast<ImageView*>(attachment.view);

            vk::RenderingAttachmentInfo attachmentInfo = {};
            attachmentInfo.setImageView(vkImageView->GetVulkanImageView());

            switch (attachment.type) {
            case Enums::RenderingAttachmentType::Color: {
                attachmentInfo.setImageLayout(vk::ImageLayout::eColorAttachmentOptimal);
                break;
            }
            case Enums::RenderingAttachmentType::DepthStencil: {
                attachmentInfo.setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
                break;
            }
            case Enums::RenderingAttachmentType::Shader: {
                attachmentInfo.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
                break;
            }
            }

            switch (attachment.lsOp) {
            case Enums::RenderingAttachmentLoadStoreOp::LoadThenStore: {
                attachmentInfo.setLoadOp(vk::AttachmentLoadOp::eLoad).setStoreOp(vk::AttachmentStoreOp::eStore);
                break;
            }
            case Enums::RenderingAttachmentLoadStoreOp::ClearThenStore: {
                attachmentInfo.setLoadOp(vk::AttachmentLoadOp::eClear).setStoreOp(vk::AttachmentStoreOp::eStore);
                break;
            }
            }

            if (attachment.type == Enums::RenderingAttachmentType::DepthStencil) {
                const auto clearDepthValue = std::get<Structs::RenderingDepthStencilClear>(attachment.clear);

                vk::ClearDepthStencilValue depthStencil = {};
                depthStencil.setDepth(clearDepthValue.depth).setStencil(clearDepthValue.stencil);
                attachmentInfo.setClearValue(depthStencil);
                depthStencilAttachmentInfo = attachmentInfo;
            }
            else {
                const auto clearColorValue = std::get<Math::Color>(attachment.clear);

                vk::ClearColorValue clearColor = {};
                clearColor.setFloat32({clearColorValue.r, clearColorValue.g, clearColorValue.b, clearColorValue.a});
                attachmentInfo.setClearValue(clearColor);
                colorAttachments.push_back(attachmentInfo);
            }
        }

        vk::Rect2D renderArea = {};
        renderArea.setOffset({renderingPass.renderArea.x, renderingPass.renderArea.y})
                  .setExtent({renderingPass.renderArea.width, renderingPass.renderArea.height});

        renderingInfo.setColorAttachments(colorAttachments)
                     .setLayerCount(1).setRenderArea(renderArea);
        if (depthStencilAttachmentInfo.imageView)
            renderingInfo.setPDepthAttachment(&depthStencilAttachmentInfo).setPStencilAttachment(
                &depthStencilAttachmentInfo);

        mCommandBuffer->beginRendering(renderingInfo);
    }

    void CommandEncoder::EndRenderingPass() { mCommandBuffer->endRendering(); }

    void CommandEncoder::MakeImageRenderable(Image* image) {

        auto& accessMasks = image->GetVulkanAccessMasks();
        auto& stageMasks = image->GetVulkanStageMasks();
        auto img = image->GetVulkanImage();
        auto& layout = image->GetVulkanLayout();

        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setBaseArrayLayer(0)
                        .setLayerCount(1)
                        .setBaseMipLevel(0)
                        .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(img)
                       .setOldLayout(layout)
                       .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
                       .setSrcAccessMask(accessMasks)
                       .setSrcStageMask(stageMasks)
                       .setDstAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
                       .setDstStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
                       .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        mCommandBuffer->pipelineBarrier2(dependencyInfo);

        layout = vk::ImageLayout::eColorAttachmentOptimal;
        accessMasks = vk::AccessFlagBits2::eColorAttachmentWrite;
        stageMasks = vk::PipelineStageFlagBits2::eColorAttachmentOutput;
    }

    void CommandEncoder::MakeImageDepthStencil(Image* image) {

        auto& accessMasks = image->GetVulkanAccessMasks();
        auto& stageMasks = image->GetVulkanStageMasks();
        auto img = image->GetVulkanImage();
        auto& layout = image->GetVulkanLayout();

        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil)
                        .setBaseArrayLayer(0)
                        .setLayerCount(1)
                        .setBaseMipLevel(0)
                        .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(img)
                       .setOldLayout(layout)
                       .setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                       .setSrcAccessMask(accessMasks)
                       .setSrcStageMask(stageMasks)
                       .setDstAccessMask(vk::AccessFlagBits2::eDepthStencilAttachmentRead |
                           vk::AccessFlagBits2::eDepthStencilAttachmentWrite)
                       .setDstStageMask(vk::PipelineStageFlagBits2::eLateFragmentTests |
                           vk::PipelineStageFlagBits2::eEarlyFragmentTests)
                       .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        mCommandBuffer->pipelineBarrier2(dependencyInfo);

        layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        accessMasks =
            vk::AccessFlagBits2::eDepthStencilAttachmentRead | vk::AccessFlagBits2::eDepthStencilAttachmentWrite;
        stageMasks = vk::PipelineStageFlagBits2::eLateFragmentTests | vk::PipelineStageFlagBits2::eEarlyFragmentTests;
    }

    void CommandEncoder::MakeImageShaderAccessible(Image* image) {

        auto& accessMasks = image->GetVulkanAccessMasks();
        auto& stageMasks = image->GetVulkanStageMasks();
        auto img = image->GetVulkanImage();
        auto& layout = image->GetVulkanLayout();

        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setBaseArrayLayer(0)
                        .setLayerCount(1)
                        .setBaseMipLevel(0)
                        .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(img)
                       .setOldLayout(layout)
                       .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                       .setSrcAccessMask(accessMasks)
                       .setSrcStageMask(stageMasks)
                       .setDstAccessMask(vk::AccessFlagBits2::eShaderRead)
                       .setDstStageMask(
                           vk::PipelineStageFlagBits2::eVertexShader | vk::PipelineStageFlagBits2::eFragmentShader)
                       .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        mCommandBuffer->pipelineBarrier2(dependencyInfo);

        layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        accessMasks = vk::AccessFlagBits2::eShaderRead;
        stageMasks = vk::PipelineStageFlagBits2::eVertexShader | vk::PipelineStageFlagBits2::eFragmentShader;
    }

    void CommandEncoder::MakeImagePresentable(Image* image) {
        auto& accessMasks = image->GetVulkanAccessMasks();
        auto& stageMasks = image->GetVulkanStageMasks();
        auto img = image->GetVulkanImage();
        auto& layout = image->GetVulkanLayout();

        vk::ImageSubresourceRange subresourceRange = {};
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setBaseArrayLayer(0)
                        .setLayerCount(1)
                        .setBaseMipLevel(0)
                        .setLevelCount(1);

        vk::ImageMemoryBarrier2 imageMemBarrier = {};
        imageMemBarrier.setImage(img)
                       .setOldLayout(layout)
                       .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
                       .setSrcAccessMask(accessMasks)
                       .setSrcStageMask(stageMasks)
                       .setDstAccessMask(vk::AccessFlagBits2::eNone)
                       .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
                       .setSubresourceRange(subresourceRange);

        vk::DependencyInfo dependencyInfo = {};
        dependencyInfo.setImageMemoryBarriers(imageMemBarrier);

        mCommandBuffer->pipelineBarrier2(dependencyInfo);

        layout = vk::ImageLayout::ePresentSrcKHR;
        accessMasks = vk::AccessFlagBits2::eNone;
        stageMasks = vk::PipelineStageFlagBits2::eNone;
    }

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

    void CommandEncoder::SetCullMode(Enums::CullMode cullMode) { mCommandBuffer->setCullMode(ToVkCullMode(cullMode)); }

    void CommandEncoder::SetDepthTesting(bool depthTesting) { mCommandBuffer->setDepthTestEnable(depthTesting); }

    void CommandEncoder::SetStencilTesting(bool stencilTesting) {
        mCommandBuffer->setStencilTestEnable(stencilTesting);
    }

    void CommandEncoder::SetDepthWriting(bool depthWriting) { mCommandBuffer->setDepthWriteEnable(depthWriting); }

    void CommandEncoder::SetViewport(const Structs::Viewport& viewport) {
        vk::Viewport vp = {
            viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth
        };
        mCommandBuffer->setViewportWithCount(1, &vp);
    }

    void CommandEncoder::SetScissor(const Structs::Rectangle& scissor) {
        vk::Rect2D s = {};
        s.setOffset({scissor.x, scissor.y})
         .setExtent({scissor.width, scissor.height});
        mCommandBuffer->setScissorWithCount(1, &s);
    }

    void CommandEncoder::BindGroup(const uint32_t groupIndex, const Vulkan::BindGroup* group, PipelineLayout* layout) {
        auto setHandle = group->GetDescriptorSet()->GetVulkanDescriptorSet();
        mCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout->GetVulkanPipelineLayout(), groupIndex, 1,
                                           &setHandle, 0, nullptr);
    }

    void CommandEncoder::DrawIndexed(uint32_t indexCount, uint32_t firstIndex) {
        mCommandBuffer->drawIndexed(indexCount, 1, firstIndex, 0, 0);
    }

    void CommandEncoder::Draw(uint32_t vertexCount, uint32_t firstIndex) {
        mCommandBuffer->draw(vertexCount, 1, firstIndex, 0);
    }
} // namespace Artus::Graphics
