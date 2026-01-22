#include <Artus/Core/Platform.h>
#include <Artus/Graphics/Device.h>

using namespace Artus;

int main() {
    const auto* window = new Core::Window();
    const auto* device = new Graphics::Device();

    while (!window->IsClosing()) {
        Core::Window::Update();
    }

    delete window;
    delete device;
}
