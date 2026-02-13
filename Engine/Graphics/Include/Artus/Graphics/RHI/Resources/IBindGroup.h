//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IBIND_GROUP_H
#define ARTUS_IBIND_GROUP_H

#include "IBindGroupLayout.h"

namespace Artus::Graphics::RHI {
    struct BindGroupBinding {
        uint32_t binding;
        std::variant<IBuffer*, IImageView*> resource;
    };

    struct BindGroupCreateDesc {
        IBindGroupLayout* layout;
        std::vector<BindGroupBinding> bindings;
    };

    class IBindGroup {
    public:
        virtual ~IBindGroup() = default;
    };
}

#endif // ARTUS_IBIND_GROUP_H
