//
// Created by fightinghawks18 on 1/26/2026.
//

#ifndef ARTUS_BUFFER_H
#define ARTUS_BUFFER_H

#include "../Utils/Vulkan/Allocator.h"
#include "Artus/Graphics/Structs.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    /// @brief Holds data that can be made accessible to the GPU
    class Buffer {
    public:
        explicit Buffer(Device& device, const Structs::BufferCreateDesc& desc);
        ~Buffer();

        void Map(size_t dataSize, size_t bufferOffset, const void* data) const;

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
