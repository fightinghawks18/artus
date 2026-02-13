//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IGRAPHICS_PIPELINE_H
#define ARTUS_IGRAPHICS_PIPELINE_H

#include "IPipelineLayout.h"
#include "IShader.h"
#include "../Types/Format.h"

#include <cstdint>
#include <vector>

namespace Artus::Graphics::RHI {
    enum class CullMode {
        Never, ///< Never cull faces
        Back, ///< Cull back faces
        Front, ///< Cull front faces
        BackAndFront, ///< Cull front and back faces
    };

    enum class DepthComparison {
        Never, ///< Never compare depth

        Less, ///< Fragment depth is less than screen pixel depth
        LessOrEqual, ///< Fragment depth is less or equal to screen pixel depth

        Greater, ///< Fragment depth is greater than screen pixel depth
        GreaterOrEqual, ///< Fragment depth is greater or equal to screen pixel depth

        Always, ///< Fragment depth is always accepted
    };

    struct GraphicsPipelineInputAttribute {
        uint32_t location;
        Format format;
        uint32_t offset;
    };

    struct GraphicsPipelineInputBinding {
        uint32_t slot;
        uint32_t stride;
        std::vector<GraphicsPipelineInputAttribute> attributes;
    };

    struct GraphicsPipelineCreateDesc {
        IShader* vertexStage;
        IShader* pixelStage;

        std::vector<GraphicsPipelineInputBinding> bindings;
        std::vector<Format> colorFormats;
        Format depthFormat;
        Format stencilFormat;
        IPipelineLayout* pipelineLayout;
        DepthComparison depthCompare;
    };

    class IGraphicsPipeline {
    public:
        virtual ~IGraphicsPipeline() = default;
    };
}

#endif // ARTUS_IGRAPHICS_PIPELINE_H
