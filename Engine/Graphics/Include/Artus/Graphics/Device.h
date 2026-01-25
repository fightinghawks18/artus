//
// Created by fightinghawks18 on 1/22/26.
//

#ifndef ARTUS_DEVICE_H
#define ARTUS_DEVICE_H

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class Device {
    public:
        explicit Device();
        ~Device();

        [[nodiscard]] vk::Instance GetVulkanInstance() { return mInstance.get(); }
        [[nodiscard]] vk::PhysicalDevice GetVulkanPhysicalDevice() const { return mPhysicalDevice; }
        [[nodiscard]] vk::Device GetVulkanDevice() { return mDevice.get(); }

        [[nodiscard]] vk::Queue GetVulkanGraphicsQueue() const { return mGraphicsQueue; }
        [[nodiscard]] vk::Queue GetVulkanComputeQueue() const { return mComputeQueue; }
        [[nodiscard]] vk::Queue GetVulkanTransferQueue() const { return mTransferQueue; }
        [[nodiscard]] uint32_t GetVulkanGraphicsFamily() const { return mGraphicsFamily; }
        [[nodiscard]] uint32_t GetVulkanComputeFamily() const { return mComputeFamily; }
        [[nodiscard]] uint32_t GetVulkanTransferFamily() const { return mTransferFamily; }
    private:
        vk::UniqueInstance mInstance;
        vk::PhysicalDevice mPhysicalDevice = nullptr;
        vk::UniqueDevice mDevice;

        vk::Queue mGraphicsQueue = nullptr;
        vk::Queue mComputeQueue = nullptr;
        vk::Queue mTransferQueue = nullptr;
        uint32_t mGraphicsFamily = UINT32_MAX;
        uint32_t mComputeFamily = UINT32_MAX;
        uint32_t mTransferFamily = UINT32_MAX;

        void MakeInstance();
        void GetPhysicalDevice();
        void MakeDevice();
    };
}

#endif // ARTUS_DEVICE_H
