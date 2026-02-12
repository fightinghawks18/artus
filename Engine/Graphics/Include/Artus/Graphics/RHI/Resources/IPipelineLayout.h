//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IPIPELINE_LAYOUT_H
#define ARTUS_IPIPELINE_LAYOUT_H

#include "IBindGroup.h"
#include "Artus/Graphics/RHI/Types/Common.h"

#include <cstdint>
#include <vector>

namespace Artus::Graphics::RHI {
    struct PipelinePushConstant {
        uint32_t size;
        uint32_t offset;
        ShaderStage shaderStages;
    };

    struct PipelineLayoutDesc {
        std::vector<IBindGroup*> groups;
        std::vector<PipelinePushConstant> pushConstants;
    };

    class IPipelineLayout {
    public:
        virtual ~IPipelineLayout() = 0;
    };
}

#endif // ARTUS_IPIPELINE_LAYOUT_H