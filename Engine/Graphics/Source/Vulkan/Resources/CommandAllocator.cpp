//
// Created by fightinghawks18 on 2/1/2026.
//

#include "Artus/Graphics/Resources/CommandAllocator.h"

namespace Artus::Graphics {
    CommandAllocator::CommandAllocator(Device& device) : mDevice(device) {
        vk::CommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
            .setQueueFamilyIndex(device.GetVulkanGraphicsFamily());

        mCmdPool = device.GetVulkanDevice().createCommandPoolUnique(cmdPoolInfo);
    }

    CommandAllocator::~CommandAllocator() {
        mCmds.clear();
        mCmdPool.reset();
    }

    CommandEncoder* CommandAllocator::NewEncoder() {
        mCmds.push_back(std::make_unique<CommandEncoder>(mDevice, mCmdPool.get()));
        return mCmds.back().get();
    }

    void CommandAllocator::DestroyEncoder(CommandEncoder* encoder) {
        std::erase_if(mCmds, [&](const std::unique_ptr<CommandEncoder>& e) { return e.get() == encoder; });
    }

    std::vector<CommandEncoder*> CommandAllocator::NewEncoders(size_t encoderCount) {
        std::vector<CommandEncoder*> encoders(encoderCount);

        for (uint32_t i = 0; i < encoderCount; i++) {
            encoders[i] = NewEncoder();
        }

        return encoders;
    }

    void CommandAllocator::DestroyEncoders(std::vector<CommandEncoder*>& encoders) {
        for (const auto& encoder : encoders) {
            DestroyEncoder(encoder);
        }

        encoders.clear();
    }
} // namespace Artus::Graphics