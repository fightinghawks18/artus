//
// Created by fightinghawks18 on 2/13/2026.
//

#include "Artus/Rendering/RenderContext.h"

namespace Artus::Rendering {
    RenderContext::RenderContext(const FrameContext& context) : mContext(context) {}

    RenderContext::~RenderContext() = default;

    void RenderContext::StartPass() {
        Graphics::Structs::RenderingAttachment mainSurfaceColorAttachment = {
            .view = mContext.surfaceFrameInfo.colorView,
            .type = Graphics::Enums::RenderingAttachmentType::Color,
            .lsOp = Graphics::Enums::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Math::Color{0.1f, 0.15f, 0.05f, 1.0f}
        };

        Graphics::Structs::RenderingPass mainPass = {
            .attachments = {mainSurfaceColorAttachment},
            .renderArea = mContext.surface->GetRectangle()
        };

        mContext.encoder->StartRenderingPass(mainPass);
    }

    void RenderContext::EndPass() {
        mContext.encoder->EndRenderingPass();
    }
}