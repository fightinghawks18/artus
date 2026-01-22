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
    private:
        vk::UniqueInstance mInstance;
        vk::PhysicalDevice mPhysicalDevice;
        vk::UniqueDevice mDevice;

        void MakeInstance();
        void GetPhysicalDevice();
        void MakeDevice();
    };
}

#endif // ARTUS_DEVICE_H
