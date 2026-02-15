//
// Created by fightinghawks18 on 2/13/2026.
//

#include "Artus/Rendering/RenderContext.h"

#include "Artus/Rendering/RenderDevice.h"

namespace Artus::Rendering {
    RenderContext::RenderContext(RenderDevice& renderDevice) : mRenderDevice(renderDevice) {}

    RenderContext::~RenderContext() = default;

    void RenderContext::StartPass(const RenderPassDesc& renderPassDesc) {
        Graphics::Structs::RenderingPass mainPass = {
            .attachments = {},
            .renderArea = mRenderDevice.GetMainSurface()->GetRectangle()
        };

        auto lsOp = Graphics::Enums::RenderingAttachmentLoadStoreOp::ClearThenStore;
        switch (renderPassDesc.loadStoreOperation) {
        case RenderPassLoadStoreOperation::LoadAndStore:
            lsOp = Graphics::Enums::RenderingAttachmentLoadStoreOp::LoadThenStore;
            break;
        case RenderPassLoadStoreOperation::ClearAndStore:
            lsOp = Graphics::Enums::RenderingAttachmentLoadStoreOp::ClearThenStore;
            break;
        }

        for (auto colorView : renderPassDesc.colorImages) {
            const Graphics::Structs::RenderingAttachment colorAttachment = {
                .view = mRenderDevice.GetImageView(colorView),
                .type = Graphics::Enums::RenderingAttachmentType::Color,
                .lsOp = lsOp,
                .clear = Math::Color{0.1f, 0.15f, 0.05f, 1.0f}
            };

            mainPass.attachments.push_back(colorAttachment);
        }

        if (renderPassDesc.depthImage) {
            auto depthViewHandle = renderPassDesc.depthImage.value();
            const Graphics::Structs::RenderingAttachment depthAttachment = {
                .view = mRenderDevice.GetImageView(depthViewHandle),
                .type = Graphics::Enums::RenderingAttachmentType::DepthStencil,
                .lsOp = lsOp,
                .clear = Graphics::Structs::RenderingDepthStencilClear{0.0f, 0}
            };

            mainPass.attachments.push_back(depthAttachment);
        }

        mRenderDevice.GetCurrentCommandEncoder()->StartRenderingPass(mainPass);
    }

    void RenderContext::EndPass() {
        mRenderDevice.GetCurrentCommandEncoder()->EndRenderingPass();
    }

    Graphics::Vulkan::CommandEncoder* RenderContext::GetRawCmd() const {
        return mRenderDevice.GetCurrentCommandEncoder();
    }
}