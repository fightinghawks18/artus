//
// Created by fightinghawks18 on 1/24/26.
//

#ifndef ARTUS_LAYER_H
#define ARTUS_LAYER_H

#ifdef __APPLE__

#include "Artus/Core/Platform.h"

namespace Artus::Graphics::Metal {
    void* CreateMetalLayer(Core::Window* window);
}

#endif // __APPLE__

#endif // ARTUS_LAYER_H