//
// Created by fightinghawks18 on 2/11/26.
//


#ifndef ARTUS_RENDERING_H
#define ARTUS_RENDERING_H

#include "Common.h"
#include "../Resources/ImageView.h"
#include <Artus/Math/Color.h>

namespace Artus::Graphics {
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
        ImageView* view;
        RenderingAttachmentType type;
        RenderingAttachmentLoadStoreOp lsOp;
        std::variant<Math::Color, RenderingDepthStencilClear> clear;
    };

    struct RenderingPass {
        std::vector<RenderingAttachment> attachments;
        Rectangle renderArea;
    };
}

#endif // ARTUS_RENDERING_H