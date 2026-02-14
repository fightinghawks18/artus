//
// Created by fightinghawks18 on 2/4/2026.
//

#ifndef ARTUS_PIPELINE_LAYOUT_H
#define ARTUS_PIPELINE_LAYOUT_H

#include "DescriptorSetLayout.h"
#include "Artus/Graphics/Structs.h"

#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class PipelineLayout {
    public:
        explicit PipelineLayout(Device& device, const Structs::PipelineLayoutCreateDesc& desc);
        ~PipelineLayout();

        [[nodiscard]] vk::PipelineLayout GetVulkanPipelineLayout() { return mPipelineLayout.get(); }

    private:
        Device& mDevice;

        vk::UniquePipelineLayout mPipelineLayout;
    };
} // namespace Artus::Graphics

#endif // ARTUS_PIPELINE_LAYOUT_H
