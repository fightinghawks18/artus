//
// Created by fightinghawks18 on 1/26/2026.
//

#include "Artus/Graphics/Vulkan/Resources/Buffer.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    Buffer::Buffer(Device& device, const Structs::BufferCreateDesc& desc) : mDevice(device) {
        auto usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
        switch (desc.usage) {
        case Enums::BufferUsage::Vertex:
            usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;
            break;
        case Enums::BufferUsage::Index:
            usageFlags = vk::BufferUsageFlagBits::eIndexBuffer;
            break;
        case Enums::BufferUsage::Shader:
            usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
            break;
        case Enums::BufferUsage::ShaderStorage:
            usageFlags = vk::BufferUsageFlagBits::eStorageBuffer;
            break;
        }

        vk::BufferCreateInfo bufferInfo = {};
        bufferInfo.setSize(desc.size).setUsage(usageFlags).setSharingMode(vk::SharingMode::eExclusive);

        VmaAllocationCreateInfo allocInfo = {0};
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
            VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        VkBuffer buffer;
        VmaAllocationInfo allocInfoOut;
        vmaCreateBuffer(device.GetVulkanAllocator(), reinterpret_cast<const VkBufferCreateInfo*>(&bufferInfo),
                        &allocInfo, &buffer, &mAllocation, &allocInfoOut);
        mBuffer = buffer;
        mMappedData = allocInfoOut.pMappedData;
    }

    Buffer::~Buffer() {
        if (!mBuffer)
            return;
        vmaDestroyBuffer(mDevice.GetVulkanAllocator(), mBuffer, mAllocation);
        mBuffer = nullptr;
        mAllocation = nullptr;
    }

    void Buffer::Map(const size_t dataSize, const size_t bufferOffset, const void* data) const {
        memcpy(static_cast<char*>(mMappedData) + bufferOffset, data, dataSize);
    }
} // namespace Artus::Graphics
