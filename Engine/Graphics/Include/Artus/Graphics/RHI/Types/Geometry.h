//
// Created by fightinghawks18 on 2/7/2026.
//


#ifndef ARTUS_GEOMETRY_H
#define ARTUS_GEOMETRY_H

#include <Artus/Math/Matrix4.h>

#include <array>

namespace Artus::Graphics::RHI {
    struct Vertex2D {
        std::array<float, 2> position;
        std::array<float, 2> uvCoords;
        std::array<float, 4> color;
    };

    struct Vertex3D {
        std::array<float, 3> position;
        std::array<float, 2> uvCoords;
        std::array<float, 4> color;
    };

    struct ModelData {
        Math::Matrix4 model;
    };

    struct CameraData {
        Math::Matrix4 view;
        Math::Matrix4 projection;
    };
}

#endif // ARTUS_GEOMETRY_H
