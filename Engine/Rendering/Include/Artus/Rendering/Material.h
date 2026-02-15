//
// Created by fightinghawks18 on 2/14/2026.
//


#ifndef ARTUS_RENDERING_MATERIAL_H
#define ARTUS_RENDERING_MATERIAL_H

#include "Artus/Graphics/Vulkan/Resources/Image.h"
#include "Utils/GraphicsAllocator.h"
#include "Utils/ResourceAllocator.h"

#include <string>

namespace Artus::Rendering {
    struct MaterialDesc {
        std::string diffuse;
    };

    struct Material {
        ImageHandle diffuse;
    };
}

#endif // ARTUS_RENDERING_MATERIAL_H