//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_RENDERING_MATERIAL_H
#define ARTUS_RENDERING_MATERIAL_H

#include "MaterialGroup.h"
#include "Utils/GraphicsAllocator.h"
#include "Utils/ResourceAllocator.h"

#include <string>

namespace Artus::Rendering {
    struct MaterialCreateDesc {
        MaterialGroupHandle materialGroup;
        std::string diffuse;
    };

    struct Material {
        MaterialGroupHandle materialGroup;
        std::vector<BindGroupHandle> bindGroups;
        std::unordered_map<std::string, ImageViewHandle> views;
        std::unordered_map<std::string, BufferHandle> buffers;
        std::unordered_map<std::string, MaterialParameter> parameters;

        ImageHandle diffuse;
    };

    using MaterialHandle = Handle<Material>;
}

#endif // ARTUS_RENDERING_MATERIAL_H