//
// Created by fightinghawks18 on 2/11/2026.
//


#ifndef ARTUS_IBUFFER_H
#define ARTUS_IBUFFER_H

#include <cstdlib>
#include <span>

namespace Artus::Graphics::RHI {
    /// @brief Describes how a buffer is used
    enum class BufferUsage {
        Vertex, ///< Buffer is used to store vertices for an object
        Index, ///< Buffer is used to store indices for an object
        Shader, ///< Buffer is read-only in shaders and stores small amounts of data
        ShaderStorage, ///< Buffer is read/write in shaders and stores large amounts of data
    };

    struct BufferDesc {
        size_t size;
        BufferUsage usage;
    };

    class IBuffer {
    public:
        virtual ~IBuffer() = 0;

        /// @brief Maps data provided to the buffer's data
        /// @warning If you try to provide data that can't fully fit into the buffer it'll cause buffer overflow
        /// @param dataSize The size of the data being mapped
        /// @param bufferOffset The offset in the buffer's data to map the data provided to
        /// @param data The data to map
        virtual void Map(size_t dataSize, size_t bufferOffset, const void* data) const = 0;

        template <typename T>
        void Map(std::span<T> data, const size_t bufferOffset = 0) {
            Map(data.size() * sizeof(T), bufferOffset, data.data());
        }
    };
}

#endif // ARTUS_IBUFFER_H
