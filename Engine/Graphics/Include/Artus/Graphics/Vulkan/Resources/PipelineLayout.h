//
// Created by fightinghawks18 on 2/4/2026.
//

#ifndef ARTUS_PIPELINE_LAYOUT_H
#define ARTUS_PIPELINE_LAYOUT_H

#include "DescriptorSetLayout.h"
#include "Artus/Graphics/RHI/Resources/IPipelineLayout.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class PipelineLayout : public RHI::IPipelineLayout {
    public:
        explicit PipelineLayout(Device& device, const RHI::PipelineLayoutDesc& desc);
        ~PipelineLayout() override;

        [[nodiscard]] vk::PipelineLayout GetVulkanPipelineLayout() { return mPipelineLayout.get(); }

    private:
        Device& mDevice;

        vk::UniquePipelineLayout mPipelineLayout;
    };
} // namespace Artus::Graphics

#endif // ARTUS_PIPELINE_LAYOUT_H
