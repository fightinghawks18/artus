//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_BIND_GROUP_H
#define ARTUS_VULKAN_BIND_GROUP_H

#include "DescriptorAllocator.h"
#include "Artus/Graphics/Structs.h"

namespace Artus::Graphics::Vulkan {
    class Device;

    class BindGroup {
    public:
        explicit BindGroup(Device& device, DescriptorAllocator* allocator, const Structs::BindGroupCreateDesc& desc);
        ~BindGroup();

        [[nodiscard]] DescriptorSet* GetDescriptorSet() const { return mSet; }

    private:
        Device& mDevice;

        DescriptorSet* mSet;
    };
}

#endif // ARTUS_VULKAN_BIND_GROUP_H
