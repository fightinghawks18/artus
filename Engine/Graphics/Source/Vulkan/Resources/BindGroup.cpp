//
// Created by fightinghawks18 on 2/12/26.
//

#include "Artus/Graphics/Vulkan/Resources/BindGroup.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"

namespace Artus::Graphics::Vulkan {
    BindGroup::BindGroup(Device& device, DescriptorAllocator* allocator, const RHI::BindGroupDesc& desc) : mDevice(
        device) {
        const auto vkBindGroupLayout = reinterpret_cast<BindGroupLayout*>(desc.layout);
        mSet = allocator->CreateDescriptorSet(vkBindGroupLayout->GetDescriptorSetLayout());
    }

    BindGroup::~BindGroup() { delete mSet; }
}
