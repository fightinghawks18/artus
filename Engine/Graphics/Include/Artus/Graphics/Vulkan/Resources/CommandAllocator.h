//
// Created by fightinghawks18 on 2/1/2026.
//

#ifndef ARTUS_COMMAND_ALLOCATOR_H
#define ARTUS_COMMAND_ALLOCATOR_H

#include "CommandEncoder.h"
#include <vulkan/vulkan.hpp>

namespace Artus::Graphics::Vulkan {
    class Device;

    class CommandAllocator {
    public:
        explicit CommandAllocator(Device& device);
        ~CommandAllocator();

        CommandEncoder* NewEncoder();
        void DestroyEncoder(CommandEncoder* encoder);

        std::vector<CommandEncoder*> NewEncoders(size_t encoderCount);
        void DestroyEncoders(std::vector<CommandEncoder*>& encoders);

    private:
        Device& mDevice;

        vk::UniqueCommandPool mCmdPool;
        std::vector<std::unique_ptr<CommandEncoder>> mCmds;
    };
} // namespace Artus::Graphics

#endif // ARTUS_COMMAND_ALLOCATOR_H
