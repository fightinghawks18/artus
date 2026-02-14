//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_COMMAND_ENCODER_H
#define ARTUS_COMMAND_ENCODER_H

#include "BindGroup.h"
#include "Buffer.h"
#include "GraphicsPipeline.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class CommandEncoder {
    public:
        explicit CommandEncoder(Device& device, vk::CommandPool commandPool);
        ~CommandEncoder();

        void Start();
        void End();
        void Reset();
        void StartRenderingPass(const Structs::RenderingPass& renderingPass);
        void EndRenderingPass();
        void MakeImageRenderable(Image* image);
        void MakeImageDepthStencil(Image* image);
        void MakeImageShaderAccessible(Image* image);
        void MakeImagePresentable(Image* image);
        void BindGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
        void BindVertexBuffer(Buffer* vertexBuffer);
        void BindIndexBuffer(Buffer* indexBuffer);
        void SetCullMode(Enums::CullMode cullMode);
        void SetDepthTesting(bool depthTesting);
        void SetStencilTesting(bool stencilTesting);
        void SetDepthWriting(bool depthWriting);
        void SetViewport(const Structs::Viewport& viewport);
        void SetScissor(const Structs::Rectangle& scissor);
        void BindGroup(uint32_t groupIndex, const BindGroup* group, PipelineLayout* layout);

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
