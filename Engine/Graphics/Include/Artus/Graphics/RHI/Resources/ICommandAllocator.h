//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ICOMMAND_ALLOCATOR_H
#define ARTUS_ICOMMAND_ALLOCATOR_H

namespace Artus::Graphics::RHI {
    class ICommandAllocator {
    public:
        virtual ~ICommandAllocator() = 0;
    };
}

#endif // ARTUS_ICOMMAND_ALLOCATOR_H