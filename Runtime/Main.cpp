#include "Artus/Graphics/Surface.h"

#include <Artus/Core/Platform.h>
#include <Artus/Graphics/Device.h>

using namespace Artus;

int main() {
    auto* window = new Core::Window();
    auto* device = new Graphics::Device();
    auto* surface = new Graphics::Surface(*device, window);

    vk::CommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
        .setQueueFamilyIndex(device->GetVulkanGraphicsFamily());

    vk::UniqueCommandPool cmdPool = device->GetVulkanDevice().createCommandPoolUnique(cmdPoolInfo);

    vk::CommandBufferAllocateInfo cmdBufferInfo = {};
    cmdBufferInfo.setCommandBufferCount(2)
        .setCommandPool(cmdPool.get());

    std::vector<vk::UniqueCommandBuffer> cmdBuffers = device->GetVulkanDevice().allocateCommandBuffersUnique(cmdBufferInfo);

    while (!window->IsClosing()) {
        Core::Window::Update();

        vk::Semaphore waitSemaphore;
        auto imageIndex = surface->AcquireNextImage(&waitSemaphore);
        auto image = surface->GetVulkanImage(imageIndex);

        vk::Rect2D rect = {};
        rect.setExtent(surface->GetVulkanExtent())
            .setOffset({0, 0});

        vk::ClearColorValue clearColor = {};
        clearColor.setFloat32({0.2f, 0.6f, 0.3f, 1.0f});

        vk::RenderingAttachmentInfo colorAttachment = {};
        colorAttachment.setClearValue(clearColor)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setImageView(surface->GetVulkanImageView(imageIndex))
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore);

        vk::RenderingInfo renderingInfo = {};
        renderingInfo.setColorAttachments(colorAttachment)
            .setLayerCount(1)
            .setRenderArea(rect);

        vk::CommandBufferBeginInfo beginInfo = {};
        beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        const auto frameIndex = surface->GetFrameIndex();

        auto cmd = cmdBuffers[frameIndex].get();
        cmd.reset();
        cmd.begin(beginInfo);

        image->MakeRenderable(cmd);

        cmd.beginRendering(renderingInfo);
        cmd.endRendering();

        image->MakePresentable(cmd);

        cmd.end();

        surface->PresentDrawn(imageIndex, cmd, waitSemaphore);
    }

    device->GetVulkanDevice().waitIdle();

    cmdBuffers.clear();
    cmdPool.reset();

    delete surface;
    delete window;
    delete device;
}
