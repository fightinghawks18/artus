//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IBIND_GROUP_H
#define ARTUS_IBIND_GROUP_H

namespace Artus::Graphics::RHI {
    class IBindGroup {
    public:
        virtual ~IBindGroup() = 0;
    };
}

#endif // ARTUS_IBIND_GROUP_H