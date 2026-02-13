//
// Created by fightinghawks18 on 2/12/2026
//

#include <Artus/Graphics/RHI/IDevice.h>
#include "Artus/Graphics/Vulkan/Device.h"
#include <Artus/Core/Window.h>

using namespace Artus;

int main() {
    auto* window = new Core::Window();
    Graphics::RHI::IDevice* device = new Graphics::Vulkan::Device();

    auto* surface = device->CreateSurface({
        .window = window
    });

    auto cmds = device->CreateCommandEncoders(3);

    while (!window->IsClosing()) {
        Core::Window::Update();

        auto* cmd = cmds[surface->GetFrameIndex()];

        const auto frameInfo = surface->PrepareFrame();
        if (!frameInfo.image || !frameInfo.view)
            continue;

        Graphics::RHI::RenderingAttachment surfaceColorAttachment = {
            .view = frameInfo.view,
            .type = Graphics::RHI::RenderingAttachmentType::Color,
            .lsOp = Graphics::RHI::RenderingAttachmentLoadStoreOp::ClearThenStore,
            .clear = Math::Color{0.0f, 0.0f, 0.0f, 1.0f}
        };

        Graphics::RHI::RenderingPass renderPass = {
            .attachments = {surfaceColorAttachment},
            .renderArea = surface->GetRectangle()
        };

        cmd->Start();
        cmd->MakeImageRenderable(frameInfo.image);
        cmd->StartRenderingPass(renderPass);
        cmd->EndRenderingPass();
        cmd->MakeImagePresentable(frameInfo.image);
        cmd->End();

        surface->PresentFrame(cmd);
    }
}