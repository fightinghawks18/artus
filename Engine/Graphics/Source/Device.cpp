//
// Created by fightinghawks18 on 1/22/26.
//

#include "Artus/Graphics/Device.h"

#include "Artus/Core/Logger.h"

#include <iostream>

struct DeviceQueue {
    vk::QueueFlags flags;
    vk::DeviceQueueCreateInfo queueInfo;
    float queuePriority[1] = {1};
};

namespace Artus::Graphics {
    Device::Device() {
        MakeInstance();
        GetPhysicalDevice();
        MakeDevice();
    }

    Device::~Device() {
        mDevice->waitIdle();

        vmaDestroyAllocator(mAllocator);
        mAllocator = nullptr;

        mDevice.reset();
        mPhysicalDevice = nullptr;
        mInstance.reset();
    }

    void Device::MakeInstance() {
        vk::ApplicationInfo appInfo = {};
        appInfo.setPApplicationName("Artus")
            .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
            .setPEngineName("ArtusEngine")
            .setEngineVersion(VK_MAKE_API_VERSION(0, 0, 0, 1))
            .setApiVersion(VK_MAKE_API_VERSION(0, 1, 3, 0));

        std::vector extensions = {
            "VK_KHR_surface"
        };
        std::vector<const char*> layers;
        vk::InstanceCreateFlags flags;

#ifdef __APPLE__
        flags |=  vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back("VK_EXT_metal_surface");
#endif

#ifndef NDEBUG
        layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        vk::InstanceCreateInfo instInfo = {};
        instInfo.setFlags(flags)
            .setPEnabledExtensionNames(extensions)
            .setPEnabledLayerNames(layers)
            .setPApplicationInfo(&appInfo);

        mInstance = vk::createInstanceUnique(instInfo);
    }

    void Device::GetPhysicalDevice() {
        const auto enumeratedDevices = mInstance->enumeratePhysicalDevices();

        int bestDeviceScore = 0;
        vk::PhysicalDevice bestDevice = nullptr;
        for (const auto& device : enumeratedDevices) {
            const auto properties = device.getProperties();
            int score = 0;

            switch (properties.deviceType) {
            case vk::PhysicalDeviceType::eIntegratedGpu:
                score += 500;
                break;
            case vk::PhysicalDeviceType::eDiscreteGpu:
                score += 1000;
                break;
            case vk::PhysicalDeviceType::eCpu:
                score += 100;
                break;
            default:
                break;
            }

            if (score > bestDeviceScore) {
                bestDeviceScore = score;
                bestDevice = device;
            }
        }

        if (bestDevice == nullptr) {
            AR_ERR("Failed to find a suitable GPU for rendering");
            return;
        }

        mPhysicalDevice = bestDevice;
        const auto properties = mPhysicalDevice.getProperties();

        AR_LOG("GPU Information:\n\tName: {}\n\tType: {}\n\tID: {}",
            std::string(properties.deviceName.data()), vk::to_string(properties.deviceType), properties.deviceID);
    }

    std::vector<DeviceQueue> FindDeviceQueues(vk::PhysicalDevice physicalDevice) {
        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        std::vector<DeviceQueue> deviceQueues;

        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            bool found = false;
            auto& queueFamily = queueFamilies[i];
            for (auto& deviceQueue : deviceQueues) {
                if (i != deviceQueue.queueInfo.queueFamilyIndex)
                    continue;
                deviceQueue.flags |= queueFamily.queueFlags;
                deviceQueue.queueInfo.queueCount++;
                found = true;
                break;
            }

            if (found)
                continue;
            vk::DeviceQueueCreateInfo queueInfo = {};
            queueInfo.setQueueCount(1)
                .setQueueFamilyIndex(i);
            deviceQueues.push_back({queueFamily.queueFlags, queueInfo});
        }

        return deviceQueues;
    }

    void Device::MakeDevice() {
        const float priority = 1;
        auto deviceQueues  = FindDeviceQueues(mPhysicalDevice);
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        for (auto& deviceQueue : deviceQueues) {
            deviceQueue.queueInfo.pQueuePriorities = &priority;
            queueCreateInfos.push_back(deviceQueue.queueInfo);
        }

        std::vector<const char*> deviceExtensions;
#ifdef __APPLE__
        deviceExtensions.push_back("VK_KHR_portability_subset");
        deviceExtensions.push_back("VK_KHR_swapchain");
#endif

        vk::PhysicalDeviceVulkan13Features v13Feats = {};
        v13Feats.setDynamicRendering(true)
            .setSynchronization2(true);

        vk::DeviceCreateInfo deviceInfo = {};
        deviceInfo.setPNext(v13Feats)
            .setQueueCreateInfos(queueCreateInfos)
            .setPEnabledExtensionNames(deviceExtensions);
        mDevice = mPhysicalDevice.createDeviceUnique(deviceInfo);

        for (auto& deviceQueue : deviceQueues) {
            if (deviceQueue.flags & vk::QueueFlagBits::eGraphics && !mGraphicsQueue) {
                mGraphicsQueue = mDevice->getQueue(deviceQueue.queueInfo.queueFamilyIndex, 0);
                mGraphicsFamily = deviceQueue.queueInfo.queueFamilyIndex;
            }

            if (deviceQueue.flags & vk::QueueFlagBits::eCompute && !mComputeQueue) {
                mComputeQueue = mDevice->getQueue(deviceQueue.queueInfo.queueFamilyIndex, 0);
                mComputeFamily = deviceQueue.queueInfo.queueFamilyIndex;
            }

            if (deviceQueue.flags & vk::QueueFlagBits::eTransfer && !mTransferQueue) {
                mTransferQueue = mDevice->getQueue(deviceQueue.queueInfo.queueFamilyIndex, 0);
                mTransferFamily = deviceQueue.queueInfo.queueFamilyIndex;
            }
        }
    }

    void Device::MakeAllocator() {
        VmaAllocatorCreateInfo allocatorInfo = {0};
        allocatorInfo.instance = mInstance.get();
        allocatorInfo.physicalDevice = mPhysicalDevice;
        allocatorInfo.device = mDevice.get();
    }
} // namespace Artus::Graphics
