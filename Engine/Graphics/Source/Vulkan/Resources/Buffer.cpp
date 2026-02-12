//
// Created by fightinghawks18 on 1/26/2026.
//

#include "Artus/Graphics/Resources/Buffer.h"

namespace Artus::Graphics {
    Buffer::Buffer(Device& device, size_t size, BufferUsage usage) : mDevice(device) {
        vk::BufferUsageFlagBits usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
        switch (usage) {
        case BufferUsage::Vertex:
            usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;
            break;
        case BufferUsage::Index:
            usageFlags = vk::BufferUsageFlagBits::eIndexBuffer;
            break;
        case BufferUsage::Shader:
            usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
            break;
        case BufferUsage::ShaderStorage:
            usageFlags = vk::BufferUsageFlagBits::eStorageBuffer;
            break;
        }

        vk::BufferCreateInfo bufferInfo = {};
        bufferInfo.setSize(size).setUsage(usageFlags).setSharingMode(vk::SharingMode::eExclusive);

        VmaAllocationCreateInfo allocInfo = {0};
        allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT ;
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        VkBuffer buffer;
        VmaAllocationInfo allocInfoOut;
        vmaCreateBuffer(device.GetVulkanAllocator(), reinterpret_cast<const VkBufferCreateInfo*>(&bufferInfo), &allocInfo, &buffer, &mAllocation, &allocInfoOut);
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

    void Buffer::Map(size_t dataSize, size_t bufferOffset, void* data) const {
        memcpy(static_cast<char*>(mMappedData) + bufferOffset, data, dataSize);
    }
} // namespace Artus::Graphics
