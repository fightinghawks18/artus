//
// Created by fightinghawks18 on 1/22/2026.
//

#include "Artus/Graphics/Vulkan/Surface.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroup.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Resources/Buffer.h"
#include "Artus/Graphics/Vulkan/Resources/GraphicsPipeline.h"
#include "Artus/Graphics/Vulkan/Resources/Image.h"
#include "Artus/Graphics/Vulkan/Resources/ImageView.h"
#include "Artus/Graphics/Vulkan/Resources/Shader.h"
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "Artus/Graphics/Vulkan/Device.h"

#include "Artus/Core/Logger.h"

struct DeviceQueue {
    vk::QueueFlags flags;
    vk::DeviceQueueCreateInfo queueInfo;
    float queuePriority[1] = {1};
};

namespace Artus::Graphics::Vulkan {
    Device::Device() {
        MakeInstance();
        GetPhysicalDevice();
        MakeDevice();
        MakeAllocator();

        mCommandAllocator = std::make_unique<CommandAllocator>(*this);

        std::vector<DescriptorAllocatorPoolDesc> poolDesc;
        poolDesc.push_back({
            .type = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 100
        });

        DescriptorAllocatorDesc allocDesc = {
            .maxDescriptorSets = 50,
            .pools = poolDesc
        };

        mDescriptorAllocator = std::make_unique<DescriptorAllocator>(*this, allocDesc);
    }

    Device::~Device() {
        mDevice->waitIdle();

        vmaDestroyAllocator(mAllocator);
        mAllocator = nullptr;

        mDevice.reset();
        mPhysicalDevice = nullptr;
        mInstance.reset();
    }

    RHI::ISurface* Device::CreateSurface(const RHI::SurfaceDesc& surfaceDesc) {
        return new Surface(*this, surfaceDesc);
    }

    RHI::IBuffer* Device::CreateBuffer(const RHI::BufferDesc& bufferDesc) { return new Buffer(*this, bufferDesc); }

    RHI::IImage* Device::CreateImage(const RHI::ImageDesc& imageDesc) { return new Image(*this, imageDesc); }

    RHI::IImageView* Device::CreateImageView(const RHI::ImageViewDesc& imageViewDesc) {
        return new ImageView(*this, imageViewDesc);
    }

    RHI::IGraphicsPipeline* Device::CreateGraphicsPipeline(const RHI::GraphicsPipelineDesc& pipelineDesc) {
        return new GraphicsPipeline(*this, pipelineDesc);
    }

    RHI::IShader* Device::CreateShader(const RHI::ShaderDesc& shaderDesc) { return new Shader(*this, shaderDesc); }

    RHI::IPipelineLayout* Device::CreatePipelineLayout(const RHI::PipelineLayoutDesc& pipelineLayout) {
        return new PipelineLayout(*this, pipelineLayout);
    }

    RHI::ICommandEncoder* Device::CreateCommandEncoder() { return mCommandAllocator->NewEncoder(); }

    std::vector<RHI::ICommandEncoder*> Device::CreateCommandEncoders(uint32_t commandEncoderCount) {
        auto cmds = mCommandAllocator->NewEncoders(commandEncoderCount);
        return std::vector<RHI::ICommandEncoder*>(cmds.begin(), cmds.end());
    }

    RHI::IBindGroupLayout* Device::CreateBindGroupLayout(const RHI::BindGroupLayoutDesc& bindGroupLayoutDesc) {
        return new BindGroupLayout(*this, mDescriptorAllocator.get(), bindGroupLayoutDesc);
    }

    RHI::IBindGroup* Device::CreateBindGroup(const RHI::BindGroupDesc& bindGroupDesc) {
        return new BindGroup(*this, mDescriptorAllocator.get(), bindGroupDesc);
    }

    void Device::MakeInstance() {
        vk::ApplicationInfo appInfo = {};
        appInfo.setPApplicationName("Artus")
               .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
               .setPEngineName("ArtusEngine")
               .setEngineVersion(VK_MAKE_API_VERSION(0, 0, 0, 1))
               .setApiVersion(VK_MAKE_API_VERSION(0, 1, 3, 0));

        std::vector extensions = {"VK_KHR_surface"};
        std::vector<const char*> layers;
        vk::InstanceCreateFlags flags;

#ifdef __APPLE__
        flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back("VK_EXT_metal_surface");
#endif
#ifdef _WIN32
        extensions.push_back("VK_KHR_win32_surface");
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

        AR_LOG("GPU Information:\n\tName: {}\n\tType: {}\n\tID: {}", std::string(properties.deviceName.data()),
               vk::to_string(properties.deviceType), properties.deviceID);
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
            queueInfo.setQueueCount(1).setQueueFamilyIndex(i);
            deviceQueues.push_back({queueFamily.queueFlags, queueInfo});
        }

        return deviceQueues;
    }

    void Device::MakeDevice() {
        const float priority = 1;
        auto deviceQueues = FindDeviceQueues(mPhysicalDevice);
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        for (auto& deviceQueue : deviceQueues) {
            deviceQueue.queueInfo.pQueuePriorities = &priority;
            queueCreateInfos.push_back(deviceQueue.queueInfo);
        }

        std::vector deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME,
        };
#ifdef __APPLE__
        deviceExtensions.push_back("VK_KHR_portability_subset");
#endif

        vk::PhysicalDeviceVulkan13Features v13Feats = {};
        v13Feats.setDynamicRendering(true).setSynchronization2(true);

        vk::PhysicalDeviceVulkan11Features v11Feats = {};
        v11Feats.setShaderDrawParameters(true).setPNext(v13Feats);

        vk::PhysicalDeviceFeatures2 deviceFeatures = {};
        deviceFeatures.setPNext(v11Feats);

        vk::DeviceCreateInfo deviceInfo = {};
        deviceInfo.setPNext(deviceFeatures).setQueueCreateInfos(queueCreateInfos).setPEnabledExtensionNames(
            deviceExtensions);
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

        vmaCreateAllocator(&allocatorInfo, &mAllocator);
    }
} // namespace Artus::Graphics
