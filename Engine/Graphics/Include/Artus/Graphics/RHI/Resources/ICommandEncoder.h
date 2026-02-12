//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_ICOMMAND_ENCODER_H
#define ARTUS_ICOMMAND_ENCODER_H

namespace Artus::Graphics::RHI {
    class ICommandEncoder {
    public:
        virtual ~ICommandEncoder() = 0;
    };
}

#endif // ARTUS_ICOMMAND_ENCODER_H
