//
// Created by fightinghawks18 on 1/22/26.
//

#include "Artus/Graphics/Device.h"

#include <iostream>

namespace Artus::Graphics {
    Device::Device() {
        MakeInstance();
        GetPhysicalDevice();
        MakeDevice();
    }

    Device::~Device() = default;

    void Device::MakeInstance() {
        vk::ApplicationInfo appInfo = {};
        appInfo.setPApplicationName("Artus")
            .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
            .setPEngineName("ArtusEngine")
            .setEngineVersion(VK_MAKE_API_VERSION(0, 0, 0, 1))
            .setApiVersion(VK_MAKE_API_VERSION(0, 1, 3, 0));

        std::vector<const char*> extensions;
        vk::InstanceCreateFlags flags;
#ifdef __APPLE__
        flags |=  vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

        vk::InstanceCreateInfo instInfo = {};
        instInfo.setFlags(flags)
            .setPEnabledExtensionNames(extensions)
            .setPEnabledLayerNames(nullptr)
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
            std::cerr << "Failed to find a suitable GPU for rendering!" << std::endl;
            return;
        }

        mPhysicalDevice = bestDevice;
        const auto properties = mPhysicalDevice.getProperties();

        std::cout << "GPU Found: {\n"
            << "\tName: " << properties.deviceName << "\n"
            << "\tType: " << vk::to_string(properties.deviceType) << "\n"
            << "\tID: " << properties.deviceID << "\n"
            << "}" << std::endl;
    }

    void Device::MakeDevice() {
        vk::DeviceCreateInfo deviceInfo = {}; // TODO: Add queues and extensions
        mDevice = mPhysicalDevice.createDeviceUnique(deviceInfo);
    }
} // namespace Artus::Graphics