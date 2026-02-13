//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_COMMAND_ENCODER_H
#define ARTUS_COMMAND_ENCODER_H

#include "BindGroup.h"
#include "Buffer.h"
#include "GraphicsPipeline.h"
#include <vulkan/vulkan.hpp>

#include "Artus/Graphics/RHI/Types/Rendering.h"
#include "Artus/Graphics/RHI/Resources/ICommandEncoder.h"

namespace Artus::Graphics::Vulkan {
    class Device;

    class CommandEncoder : public RHI::ICommandEncoder {
    public:
        explicit CommandEncoder(Device& device, vk::CommandPool commandPool);
        ~CommandEncoder() override;

        void Start() override;
        void End() override;
        void Reset() override;
        void StartRenderingPass(const RHI::RenderingPass& renderingPass) override;
        void EndRenderingPass() override;
        void MakeImageRenderable(RHI::IImage* image) override;
        void MakeImageDepthStencil(RHI::IImage* image) override;
        void MakeImageShaderAccessible(RHI::IImage* image) override;
        void MakeImagePresentable(RHI::IImage* image) override;
        void BindGraphicsPipeline(RHI::IGraphicsPipeline* graphicsPipeline) override;
        void BindVertexBuffer(RHI::IBuffer* vertexBuffer) override;
        void BindIndexBuffer(RHI::IBuffer* indexBuffer) override;
        void SetCullMode(RHI::CullMode cullMode) override;
        void SetDepthTesting(bool depthTesting) override;
        void SetStencilTesting(bool stencilTesting) override;
        void SetDepthWriting(bool depthWriting) override;
        void SetViewport(const RHI::Viewport& viewport) override;
        void SetScissor(const RHI::Rectangle& scissor) override;
        void BindGroup(RHI::IBindGroup* group, RHI::IPipelineLayout* layout) override;

        void DrawIndexed(uint32_t indexCount, uint32_t firstIndex) override;
        void Draw(uint32_t vertexCount, uint32_t firstIndex) override;

        [[nodiscard]] vk::CommandBuffer GetVulkanCommandBuffer() { return mCommandBuffer.get(); }
        [[nodiscard]] bool InUse() const { return mInUse; }

    private:
        Device& mDevice;

        vk::UniqueCommandBuffer mCommandBuffer;
        bool mInUse = false;
    };
} // namespace Artus::Graphics

#endif // ARTUS_COMMAND_ENCODER_H
