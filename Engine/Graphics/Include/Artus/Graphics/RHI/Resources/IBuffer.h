//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IBUFFER_H
#define ARTUS_IBUFFER_H

#include <cstdint>

namespace Artus::Graphics::RHI {
    /// @brief Describes how a buffer is used
    enum class BufferUsage {
        Vertex,        ///< Buffer is used to store vertices for an object
        Index,         ///< Buffer is used to store indices for an object
        Shader,        ///< Buffer is read-only in shaders and stores small amounts of data
        ShaderStorage, ///< Buffer is read/write in shaders and stores large amounts of data
    };

    struct BufferDesc {
        size_t size;
        BufferUsage usage;
    };

    class IBuffer {
    public:
        virtual ~IBuffer() = 0;
    };
}

#endif // ARTUS_IBUFFER_H