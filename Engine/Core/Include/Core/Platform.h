//
// Created by fightinghawks18 on 1/21/26.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include <memory>

namespace Artus::Core {
    class Window {
    public:
        static void Update();
        
        explicit Window();
        ~Window();
        
        void SetClosing(const bool closing) { mClosing = closing; }
        
        [[nodiscard]] bool IsClosing() const { return mClosing; }
    private:
        struct Impl;
        std::unique_ptr<Impl> mImpl;
        bool mClosing;
    };
}

#endif // PLATFORM_H

