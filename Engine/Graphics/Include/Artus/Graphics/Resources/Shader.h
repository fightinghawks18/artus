//
// Created by fightinghawks18 on 1/27/2026
//

#ifndef ARTUS_SHADER_H
#define ARTUS_SHADER_H

#include "Artus/Graphics/Device.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics {
    class Shader {
      public:
        explicit Shader(Device& device, const std::string& shaderPath);
        ~Shader();

        [[nodiscard]] vk::ShaderModule GetVulkanShaderModule() const { return mShaderModule.get(); }
      private:
        Device& mDevice;

        vk::UniqueShaderModule mShaderModule;
    };
} // namespace Artus::Graphics

#endif