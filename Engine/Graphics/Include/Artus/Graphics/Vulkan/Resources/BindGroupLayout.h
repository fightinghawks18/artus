//
// Created by fightinghawks18 on 2/12/26.
//


#ifndef ARTUS_VULKAN_BIND_GROUP_LAYOUT_H
#define ARTUS_VULKAN_BIND_GROUP_LAYOUT_H

#include "DescriptorAllocator.h"
#include "DescriptorSetLayout.h"
#include "Artus/Graphics/RHI/Resources/IBindGroupLayout.h"
#include "Artus/Graphics/Vulkan/Device.h"

namespace Artus::Graphics::Vulkan {
    class BindGroupLayout : public RHI::IBindGroupLayout {
    public:
        explicit BindGroupLayout(Device& device, DescriptorAllocator* allocator, const RHI::BindGroupLayoutDesc& desc);
        ~BindGroupLayout() override;

        [[nodiscard]] DescriptorSetLayout* GetDescriptorSetLayout() const { return mLayout; }

    private:
        Device& mDevice;

        DescriptorSetLayout* mLayout;
    };
}

#endif // ARTUS_VULKAN_BIND_GROUP_LAYOUT_H
