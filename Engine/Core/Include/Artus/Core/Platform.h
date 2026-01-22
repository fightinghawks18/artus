//
// Created by fightinghawks18 on 1/21/26.
//

#ifndef ARTUS_PLATFORM_H
#define ARTUS_PLATFORM_H

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

#endif // ARTUS_PLATFORM_H

