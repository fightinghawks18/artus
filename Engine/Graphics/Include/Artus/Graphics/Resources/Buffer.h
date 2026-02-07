//
// Created by fightinghawks18 on 1/26/2026.
//

#ifndef ARTUS_BUFFER_H
#define ARTUS_BUFFER_H

#include "Artus/Graphics/Device.h"

#include "Artus/Graphics/Utils/Vulkan/Allocator.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    /// @brief Describes how a buffer is used
    enum class BufferUsage {
        Vertex,        ///< Buffer is used to store vertices for an object
        Index,         ///< Buffer is used to store indices for an object
        Shader,        ///< Buffer is read-only in shaders and stores small amounts of data
        ShaderStorage, ///< Buffer is read/write in shaders and stores large amounts of data
    };

    /// @brief Holds data that can be made accessible to the GPU
    class Buffer {
    public:
        explicit Buffer(Device& device, size_t size, BufferUsage usage);
        ~Buffer();

        /// @brief Maps data provided to the buffer's data
        /// @warning If you try to provide data that can't fully fit into the buffer it'll cause buffer overflow
        /// @param dataSize The size of the data being mapped
        /// @param bufferOffset The offset in the buffer's data to map the data provided to
        /// @param data The data to map
        void Map(size_t dataSize, size_t bufferOffset, void* data) const;

        /// @brief Retrieves the underlying vulkan handle to this buffer
        /// @return vk::Buffer
        [[nodiscard]] vk::Buffer GetVulkanBuffer() const { return mBuffer; }
        /// @brief Retrieves the underlying vulkan allocation to this buffer
        /// @return VmaAllocation
        [[nodiscard]] VmaAllocation GetVulkanAllocation() const { return mAllocation; }

    private:
        Device& mDevice;

        vk::Buffer mBuffer;
        VmaAllocation mAllocation;
        void* mMappedData = nullptr;
    };
} // namespace Artus::Graphics

#endif // ARTUS_BUFFER_H
