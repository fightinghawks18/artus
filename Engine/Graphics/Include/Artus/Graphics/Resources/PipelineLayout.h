//
// Created by fightinghawks18 on 2/4/2026.
//

#ifndef ARTUS_PIPELINE_LAYOUT_H
#define ARTUS_PIPELINE_LAYOUT_H

#include "../Device.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class PipelineLayout {
    public:
        explicit PipelineLayout(Device& device);
        ~PipelineLayout();

        [[nodiscard]] vk::PipelineLayout GetVulkanPipelineLayout() { return mPipelineLayout.get(); }

    private:
        Device& mDevice;

        vk::UniquePipelineLayout mPipelineLayout;
    };
} // namespace Artus::Graphics

#endif // ARTUS_PIPELINE_LAYOUT_H