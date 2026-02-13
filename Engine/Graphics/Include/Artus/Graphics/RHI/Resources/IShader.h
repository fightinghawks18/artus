//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ISHADER_H
#define ARTUS_ISHADER_H

#include <string>

namespace Artus::Graphics::RHI {
    struct ShaderCreateDesc {
        std::string path;
    };

    class IShader {
    public:
        virtual ~IShader() = default;
    };
}

#endif // ARTUS_ISHADER_H
