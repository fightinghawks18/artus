//
// Created by fightinghawks18 on 1/22/2026.
//

#ifndef ARTUS_DEVICE_H
#define ARTUS_DEVICE_H

#include "Utils/Vulkan/Allocator.h"

#include "Surface.h"
#include "Resources/Buffer.h"
#include "Resources/Image.h"
#include "Resources/ImageView.h"
#include "Resources/GraphicsPipeline.h"
#include "Resources/Shader.h"
#include "Resources/PipelineLayout.h"
#include "Resources/CommandAllocator.h"
#include "Resources/DescriptorAllocator.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device {
    public:
        explicit Device();
        ~Device();

        Surface* CreateSurface(const Structs::SurfaceCreateDesc& surfaceCreateDesc);
        Buffer* CreateBuffer(const Structs::BufferCreateDesc& bufferCreateDesc);
        Image* CreateImage(const Structs::ImageCreateDesc& imageCreateDesc);
        ImageView* CreateImageView(const Structs::ImageViewCreateDesc& imageViewCreateDesc);
        GraphicsPipeline* CreateGraphicsPipeline(const Structs::GraphicsPipelineCreateDesc& pipelineCreateDesc);
        Shader* CreateShader(const Structs::ShaderCreateDesc& shaderCreateDesc);
        PipelineLayout* CreatePipelineLayout(const Structs::PipelineLayoutCreateDesc& pipelineCreateLayout);
        CommandEncoder* CreateCommandEncoder();
        std::vector<CommandEncoder*> CreateCommandEncoders(uint32_t commandEncoderCount);
        BindGroupLayout* CreateBindGroupLayout(const Structs::BindGroupLayoutCreateDesc& bindGroupLayoutCreateDesc);
        BindGroup* CreateBindGroup(const Structs::BindGroupCreateDesc& bindGroupCreateDesc);

        [[nodiscard]] vk::Instance GetVulkanInstance() { return mInstance.get(); }
        [[nodiscard]] vk::PhysicalDevice GetVulkanPhysicalDevice() const { return mPhysicalDevice; }
        [[nodiscard]] vk::Device GetVulkanDevice() { return mDevice.get(); }
        [[nodiscard]] VmaAllocator GetVulkanAllocator() const { return mAllocator; }

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
        VmaAllocator mAllocator = nullptr;

        vk::Queue mGraphicsQueue = nullptr;
        vk::Queue mComputeQueue = nullptr;
        vk::Queue mTransferQueue = nullptr;
        uint32_t mGraphicsFamily = UINT32_MAX;
        uint32_t mComputeFamily = UINT32_MAX;
        uint32_t mTransferFamily = UINT32_MAX;

        std::unique_ptr<DescriptorAllocator> mDescriptorAllocator;
        std::unique_ptr<CommandAllocator> mCommandAllocator;

        void MakeInstance();
        void GetPhysicalDevice();
        void MakeDevice();
        void MakeAllocator();
    };
} // namespace Artus::Graphics

#endif // ARTUS_DEVICE_H
