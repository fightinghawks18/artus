//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ICOMMAND_ENCODER_H
#define ARTUS_ICOMMAND_ENCODER_H

#include "../Types/Rendering.h"
#include "IGraphicsPipeline.h"
#include "IBindGroup.h"

namespace Artus::Graphics::RHI {
    class ICommandEncoder {
    public:
        virtual ~ICommandEncoder() = default;

        virtual void Start() = 0;
        virtual void End() = 0;
        virtual void Reset() = 0;
        virtual void StartRenderingPass(const RenderingPass& renderingPass) = 0;
        virtual void EndRenderingPass() = 0;
        virtual void MakeImageRenderable(IImage* image) = 0;
        virtual void MakeImageDepthStencil(IImage* image) = 0;
        virtual void MakeImageShaderAccessible(IImage* image) = 0;
        virtual void MakeImagePresentable(IImage* image) = 0;
        virtual void BindGraphicsPipeline(IGraphicsPipeline* graphicsPipeline) = 0;
        virtual void BindVertexBuffer(IBuffer* vertexBuffer) = 0;
        virtual void BindIndexBuffer(IBuffer* indexBuffer) = 0;
        virtual void SetCullMode(CullMode cullMode) = 0;
        virtual void SetDepthTesting(bool depthTesting) = 0;
        virtual void SetStencilTesting(bool stencilTesting) = 0;
        virtual void SetDepthWriting(bool depthWriting) = 0;
        virtual void SetViewport(const Viewport& viewport) = 0;
        virtual void SetScissor(const Rectangle& scissor) = 0;
        virtual void BindGroup(IBindGroup* group, IPipelineLayout* layout) = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t firstIndex) = 0;
        virtual void Draw(uint32_t vertexCount, uint32_t firstIndex) = 0;
    };
}

#endif // ARTUS_ICOMMAND_ENCODER_H
