//
// Created by fightinghawks18 on 1/28/2026
//

#include "Artus/Graphics/Resources/Shader.h"

#include <fstream>

static std::vector<uint32_t> ReadSpirv(const std::string& shader) {
    std::ifstream file(shader, std::ios::binary | std::ios::ate);

    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint32_t> buffer(size / sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    return buffer;
}

namespace Artus::Graphics {
    Shader::Shader(Device& device, const std::string& shaderPath) : mDevice(device) {
        auto shaderBuffer = ReadSpirv(shaderPath);

        vk::ShaderModuleCreateInfo shaderModuleInfo = {};
        shaderModuleInfo.setCode(shaderBuffer);

        mShaderModule = mDevice.GetVulkanDevice().createShaderModuleUnique(shaderModuleInfo);
    }

    Shader::~Shader() = default;
} // namespace Artus::Graphics