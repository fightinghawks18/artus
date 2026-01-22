#include <Core/Platform.h>

int main() {
    auto* window = new Artus::Core::Window();
    
    while (!window->IsClosing()) {
        Artus::Core::Window::Update();
    }
    delete window;
}
