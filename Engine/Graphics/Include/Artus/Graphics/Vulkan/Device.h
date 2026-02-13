//
// Created by fightinghawks18 on 1/22/2026.
//

#ifndef ARTUS_DEVICE_H
#define ARTUS_DEVICE_H

#include "../RHI/IDevice.h"
#include "Artus/Graphics/Vulkan/Utils/Vulkan/Allocator.h"
#include <vulkan/vulkan.hpp>

#include "Resources/CommandAllocator.h"
#include "Resources/DescriptorAllocator.h"

namespace Artus::Graphics::Vulkan {
    class Device : public RHI::IDevice {
    public:
        explicit Device();
        ~Device() override;

        RHI::ISurface* CreateSurface(const RHI::SurfaceCreateDesc& surfaceCreateDesc) override;
        RHI::IBuffer* CreateBuffer(const RHI::BufferCreateDesc& bufferCreateDesc) override;
        RHI::IImage* CreateImage(const RHI::ImageCreateDesc& imageCreateDesc) override;
        RHI::IImageView* CreateImageView(const RHI::ImageViewCreateDesc& imageViewDesc) override;
        RHI::IGraphicsPipeline* CreateGraphicsPipeline(const RHI::GraphicsPipelineCreateDesc& pipelineDesc) override;
        RHI::IShader* CreateShader(const RHI::ShaderCreateDesc& shaderDesc) override;
        RHI::IPipelineLayout* CreatePipelineLayout(const RHI::PipelineLayoutCreateDesc& pipelineLayout) override;
        RHI::ICommandEncoder* CreateCommandEncoder() override;
        std::vector<RHI::ICommandEncoder*> CreateCommandEncoders(uint32_t commandEncoderCount) override;
        RHI::IBindGroupLayout* CreateBindGroupLayout(const RHI::BindGroupLayoutCreateDesc& bindGroupLayoutDesc) override;
        RHI::IBindGroup* CreateBindGroup(const RHI::BindGroupCreateDesc& bindGroupDesc) override;

        [[nodiscard]] vk::Instance GetVulkanInstance() { return mInstance.get(); }
        [[nodiscard]] vk::PhysicalDevice GetVulkanPhysicalDevice() const { return mPhysicalDevice; }
        [[nodiscard]] vk::Device GetVulkanDevice() { return mDevice.get(); }
        [[nodiscard]] VmaAllocator GetVulkanAllocator() { return mAllocator; }

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
