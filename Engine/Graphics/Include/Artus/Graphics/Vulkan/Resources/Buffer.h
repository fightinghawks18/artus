//
// Created by fightinghawks18 on 1/26/2026.
//

#ifndef ARTUS_BUFFER_H
#define ARTUS_BUFFER_H

#include "../Utils/Vulkan/Allocator.h"
#include "Artus/Graphics/RHI/Resources/IBuffer.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    /// @brief Holds data that can be made accessible to the GPU
    class Buffer : public RHI::IBuffer {
    public:
        explicit Buffer(Device& device, const RHI::BufferDesc& desc);
        ~Buffer() override;

        void Map(size_t dataSize, size_t bufferOffset, const void* data) const override;

        /// @brief Retrieves the underlying vulkan handle to this buffer
        /// @return vk::Buffer
        [[nodiscard]] vk::Buffer GetVulkanBuffer() const { return mBuffer; }
        /// @brief Retrieves the underlying vulkan allocation to this buffer
        /// @return VmaAllocation
        [[nodiscard]] VmaAllocation GetVulkanAllocation() const { return mAllocation; }

    private:
        Vulkan::Device& mDevice;

        vk::Buffer mBuffer;
        VmaAllocation mAllocation;
        void* mMappedData = nullptr;
    };
} // namespace Artus::Graphics

#endif // ARTUS_BUFFER_H
