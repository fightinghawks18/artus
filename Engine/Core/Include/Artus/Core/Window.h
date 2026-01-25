//
// Created by fightinghawks18 on 1/21/26.
//

#ifndef ARTUS_PLATFORM_H
#define ARTUS_PLATFORM_H

#include <memory>

namespace Artus::Core {
    struct WindowHandle { void* handle; };
    struct WindowSize {
        int32_t width;
        int32_t height;
    };

    class Window {
    public:
        static void Update();
        
        explicit Window();
        ~Window();
        
        void SetClosing(const bool closing) { mClosing = closing; }
        
        [[nodiscard]] WindowSize GetWindowSize();
        [[nodiscard]] WindowSize GetWindowContentSize();
        [[nodiscard]] bool IsClosing() const { return mClosing; }
        [[nodiscard]] WindowHandle GetHandle();
    private:
        struct Impl;
        std::unique_ptr<Impl> mImpl;
        bool mClosing;
    };
}

#endif // ARTUS_PLATFORM_H

