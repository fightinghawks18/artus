//
// Created by fightinghawks18 on 2/12/26.
//

#include "Artus/Graphics/Vulkan/Resources/BindGroup.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Utils/Common/Pipeline.h"

namespace Artus::Graphics::Vulkan {
    BindGroup::BindGroup(Device& device, DescriptorAllocator* allocator, const Structs::BindGroupCreateDesc& desc) : mDevice(
        device) {
        mSet = allocator->CreateDescriptorSet(desc.layout->GetDescriptorSetLayout());

        for (const auto& binding : desc.bindings) {
            auto bindingType = Enums::BindGroupLayoutBindingType::Buffer;

            for (const auto& layoutBinding : desc.layout->mCreateDesc.bindings) {
                if (layoutBinding.binding == binding.binding) {
                    bindingType = layoutBinding.type;
                    break;
                }
            }

            switch (bindingType) {
            case Enums::BindGroupLayoutBindingType::Buffer: {
                mSet->WriteDescriptorSet(
                    binding.binding,
                    0,
                    ToVkDescriptorType(bindingType),
                    std::get<Buffer*>(binding.resource));
                break;
            }
            case Enums::BindGroupLayoutBindingType::ImageView: {
                auto imageView = std::get<ImageView*>(binding.resource);
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
