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
    struct PipelineLayoutDesc {
        std::vector<IBindGroupLayout*> groupLayouts;
    };

    class IPipelineLayout {
    public:
        virtual ~IPipelineLayout() = default;
    };
}

#endif // ARTUS_IPIPELINE_LAYOUT_H
