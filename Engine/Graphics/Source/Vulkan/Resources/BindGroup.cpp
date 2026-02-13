//
// Created by fightinghawks18 on 2/12/26.
//

#include "Artus/Graphics/Vulkan/Resources/BindGroup.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Utils/Common/Pipeline.h"

namespace Artus::Graphics::Vulkan {
    BindGroup::BindGroup(Device& device, DescriptorAllocator* allocator, const RHI::BindGroupCreateDesc& desc) : mDevice(
        device) {
        const auto vkBindGroupLayout = reinterpret_cast<BindGroupLayout*>(desc.layout);
        mSet = allocator->CreateDescriptorSet(vkBindGroupLayout->GetDescriptorSetLayout());

        for (const auto& binding : desc.bindings) {
            auto bindingType = RHI::BindGroupLayoutBindingType::Buffer;

            for (const auto& layoutBinding : vkBindGroupLayout->GetDesc().bindings) {
                if (layoutBinding.binding == binding.binding) {
                    bindingType = layoutBinding.type;
                    break;
                }
            }

            switch (bindingType) {
            case RHI::BindGroupLayoutBindingType::Buffer: {
                mSet->WriteDescriptorSet(
                    binding.binding,
                    0,
                    ToVkDescriptorType(bindingType),
                    dynamic_cast<const Buffer*>(std::get<RHI::IBuffer*>(binding.resource)));
                break;
            }
            case RHI::BindGroupLayoutBindingType::ImageView: {
                auto imageView = dynamic_cast<const ImageView*>(std::get<RHI::IImageView*>(binding.resource));
                mSet->WriteDescriptorSet(
                                    binding.binding,
                                    0,
                                    ToVkDescriptorType(bindingType),
                                    imageView,
                                    vk::ImageLayout::eShaderReadOnlyOptimal);
                break;
            }
            }
        }
    }

    BindGroup::~BindGroup() { delete mSet; }
}
