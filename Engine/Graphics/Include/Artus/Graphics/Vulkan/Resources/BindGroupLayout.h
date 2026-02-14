//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_BIND_GROUP_LAYOUT_H
#define ARTUS_VULKAN_BIND_GROUP_LAYOUT_H

#include "DescriptorAllocator.h"
#include "DescriptorSetLayout.h"
#include "Artus/Graphics/Structs.h"

namespace Artus::Graphics::Vulkan {
    class Device;

    class BindGroupLayout {
    public:
        explicit BindGroupLayout(Device& device, DescriptorAllocator* allocator, const Structs::BindGroupLayoutCreateDesc& desc);
        ~BindGroupLayout();

        [[nodiscard]] DescriptorSetLayout* GetDescriptorSetLayout() const { return mLayout; }

    private:
        Device& mDevice;
        DescriptorSetLayout* mLayout;
        Structs::BindGroupLayoutCreateDesc mCreateDesc;

        friend class BindGroup;
    };
}

#endif // ARTUS_VULKAN_BIND_GROUP_LAYOUT_H
