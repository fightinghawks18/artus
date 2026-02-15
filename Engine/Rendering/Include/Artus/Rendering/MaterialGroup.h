//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_RENDERING_MATERIAL_GROUP_H
#define ARTUS_RENDERING_MATERIAL_GROUP_H

#include "Artus/Core/Logger.h"
#include "Utils/GraphicsAllocator.h"

#include <unordered_map>

namespace Artus::Rendering {
    using MaterialParameterValue = std::variant<uint32_t, float, int32_t>;

    struct MaterialParameter {
        uint32_t bindGroup;
        MaterialParameterValue value;
    };

    struct MaterialGroupCreateDesc {
        GraphicsPipelineCreateDesc pipelineCreateDesc;
        PipelineLayoutCreateDesc pipelineLayoutCreateDesc;
        std::vector<BindGroupLayoutCreateDesc> bindGroupLayoutCreateDescs;
        std::vector<BindGroupCreateDesc> bindGroupCreateDescs;
    };

    struct MaterialGroup {
        GraphicsPipelineHandle pipeline;
        PipelineLayoutHandle pipelineLayout;
        std::vector<BindGroupLayoutCreateDesc> layoutCreateDescs;
        std::vector<BindGroupLayoutHandle> bindGroupLayouts;
        std::unordered_map<std::string, MaterialParameter> parameters;

        MaterialGroup& SetParameter(const std::string& parameterName, MaterialParameterValue value) {
            if (!parameters.contains(parameterName)) {
                AR_ERR("Attempted to access invalid parameter {} in MaterialGroup!", parameterName);
                return *this;
            }

            parameters[parameterName].value = value;
            return *this;
        }
    };

    using MaterialGroupHandle = Handle<MaterialGroup>;

}

#endif // ARTUS_RENDERING_MATERIAL_GROUP_H