#include "Artus/Graphics/Surface.h"

#include <Artus/Core/Window.h>
#include <Artus/Graphics/Device.h>
#include <Artus/Graphics/Resources/CommandAllocator.h>

using namespace Artus;

int main() {
    auto* window = new Core::Window();
    auto* device = new Graphics::Device();
    auto* surface = new Graphics::Surface(*device, window);
    auto* cmdAllocator = new Graphics::CommandAllocator(*device);
    auto cmdEncoders = cmdAllocator->NewEncoders(3);

    while (!window->IsClosing()) {
        Core::Window::Update();

        vk::Semaphore waitSemaphore;
        auto imageIndex = surface->AcquireNextImage(&waitSemaphore);
        auto image = surface->GetVulkanImage(imageIndex);

        vk::Rect2D rect = {};
        rect.setExtent(surface->GetVulkanExtent()).setOffset({0, 0});

        vk::ClearColorValue clearColor = {};
        clearColor.setFloat32({0.2f, 0.6f, 0.3f, 1.0f});

        vk::RenderingAttachmentInfo colorAttachment = {};
        colorAttachment.setClearValue(clearColor)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setImageView(surface->GetVulkanImageView(imageIndex))
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore);

        vk::RenderingInfo renderingInfo = {};
        renderingInfo.setColorAttachments(colorAttachment).setLayerCount(1).setRenderArea(rect);

        const auto frameIndex = surface->GetFrameIndex();

        auto cmd = cmdEncoders[frameIndex];
        cmd->Reset();
        cmd->Start();

        image->MakeRenderable(cmd->GetVulkanCommandBuffer());

        cmd->StartRendering(renderingInfo);
        cmd->EndRendering();

        image->MakePresentable(cmd->GetVulkanCommandBuffer());

        cmd->End();

        surface->PresentDrawn(imageIndex, cmd->GetVulkanCommandBuffer(), waitSemaphore);
    }

    device->GetVulkanDevice().waitIdle();

    delete cmdAllocator;

    delete surface;
    delete window;
    delete device;
}
