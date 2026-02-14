//
// Created by fightinghawks18 on 2/12/26.
//

#include "Artus/Graphics/Vulkan/Resources/BindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Device.h"
#include "Artus/Graphics/Vulkan/Utils/Common/Pipeline.h"

namespace Artus::Graphics::Vulkan {
    BindGroupLayout::BindGroupLayout(Device& device, DescriptorAllocator* allocator,
                                     const Structs::BindGroupLayoutCreateDesc& desc) : mDevice(device), mCreateDesc(desc) {
        std::vector<DescriptorSetLayoutBinding> bindings;
        for (const auto& binding : desc.bindings) {
            DescriptorSetLayoutBinding layoutBinding = {
                .binding = binding.binding,
                .type = ToVkDescriptorType(binding.type),
                .stageFlags = ToVkShaderStageFlags(binding.visibility),
                .count = 1 // WebGPU approach doesn't support multiple descriptors in one binding
            };

            bindings.push_back(layoutBinding);
        }

        mLayout = allocator->CreateDescriptorSetLayout(bindings);
    }

    BindGroupLayout::~BindGroupLayout() { delete mLayout; }
}
