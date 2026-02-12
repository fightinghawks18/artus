//
// Created by fightinghawks18 on 2/11/26.
//


#ifndef ARTUS_RENDERING_H
#define ARTUS_RENDERING_H

#include "Common.h"
#include "../Resources/IImageView.h"
#include <Artus/Math/Color.h>

#include <variant>
#include <vector>

namespace Artus::Graphics::RHI {
    struct RenderingDepthStencilClear {
        float depth;
        uint32_t stencil;
    };

    enum class RenderingAttachmentLoadStoreOp {
        LoadThenStore,
        ClearThenStore,
    };

    enum class RenderingAttachmentType {
        Color,
        DepthStencil,
        Shader
    };

    struct RenderingAttachment {
        IImageView* view = nullptr;
        RenderingAttachmentType type = RenderingAttachmentType::Color;
        RenderingAttachmentLoadStoreOp lsOp = RenderingAttachmentLoadStoreOp::ClearThenStore;
        std::variant<Math::Color, RenderingDepthStencilClear> clear;
    };

    struct RenderingPass {
        std::vector<RenderingAttachment> attachments;
        Rectangle renderArea;
    };
}

#endif // ARTUS_RENDERING_H
