//
// Created by fightinghawks18 on 1/27/2026.
//

#ifndef ARTUS_SHADER_H
#define ARTUS_SHADER_H

#include "Artus/Graphics/RHI/Resources/IShader.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class Shader : public RHI::IShader {
    public:
        explicit Shader(Device& device, const RHI::ShaderCreateDesc& desc);
        ~Shader() override;

        [[nodiscard]] vk::ShaderModule GetVulkanShaderModule() const { return mShaderModule.get(); }

    private:
        Device& mDevice;

        vk::UniqueShaderModule mShaderModule;
    };
} // namespace Artus::Graphics

#endif
