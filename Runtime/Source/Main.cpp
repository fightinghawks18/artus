//
// Created by fightinghawks18 on 2/12/2026
//

#include <Artus/Rendering/RenderDevice.h>
#include <Artus/Core/Window.h>
#include <Artus/Math/Utilities.h>

using namespace Artus;

int main() {
    auto* window = new Core::Window();

    auto* renderDevice = new Rendering::RenderDevice({ .renderingApi = Rendering::RenderAPI::Vulkan, .mainWindow = window });

    while (!window->IsClosing()) {
        Core::Window::Update();

        auto ctx = renderDevice->StartRendering();
        ctx.StartPass();
        ctx.EndPass();
        renderDevice->EndRendering(ctx);
    }
}