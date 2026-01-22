//
// Created by fightinghawks18 on 1/22/26.
//

#include "Artus/Graphics/Device.h"

namespace Artus::Graphics {
    Device::Device() {
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

    Device::~Device() = default;
} // namespace Artus::Graphics