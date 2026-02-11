//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_COMMAND_ENCODER_H
#define ARTUS_COMMAND_ENCODER_H

#include "../Device.h"
#include "Buffer.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"
#include <vulkan/vulkan.hpp>

#include "Artus/Graphics/Types/Rendering.h"

namespace Artus::Graphics {
    class CommandEncoder {
    public:
        explicit CommandEncoder(Device& device, vk::CommandPool commandPool);
        ~CommandEncoder();

        void Start();
        void End();

        void Reset();
        void StartRenderingPass(const RenderingPass& renderingPass);
        void EndRenderingPass();

        void MakeImageRenderable(Image* image);
        void MakeImageDepthStencil(Image* image);
        void MakeImageShaderAccessible(Image* image);
        void MakeImagePresentable(Image* image);

        void BindGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
        void BindVertexBuffer(Buffer* vertexBuffer);
        void BindIndexBuffer(Buffer* indexBuffer);
        void SetCullMode(vk::CullModeFlags cullMode);
        void SetDepthTesting(bool depthTesting);
        void SetStencilTesting(bool stencilTesting);
        void SetDepthWriting(bool depthWriting);
        void SetViewport(const Viewport& viewport);
        void SetScissor(const Rectangle& scissor);
        void BindDescriptorSet(DescriptorSet* set, PipelineLayout* layout);
        void UpdatePushConstant(PipelineLayout* layout, vk::ShaderStageFlagBits stageFlags, uint32_t size, uint32_t offset, void* data);

        void DrawIndexed(uint32_t indexCount, uint32_t firstIndex);
        void Draw(uint32_t vertexCount, uint32_t firstIndex);

        [[nodiscard]] vk::CommandBuffer GetVulkanCommandBuffer() { return mCommandBuffer.get(); }
        [[nodiscard]] bool InUse() const { return mInUse; }

    private:
        Device& mDevice;

        vk::UniqueCommandBuffer mCommandBuffer;
        bool mInUse = false;
    };
} // namespace Artus::Graphics

#endif // ARTUS_COMMAND_ENCODER_H