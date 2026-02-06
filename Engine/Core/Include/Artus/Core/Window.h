//
// Created by fightinghawks18 on 1/21/2026.
//

#ifndef ARTUS_PLATFORM_H
#define ARTUS_PLATFORM_H

#include <memory>

namespace Artus::Core {
    /// @brief A pointer to the window's underlying platform handle
    struct WindowHandle {
        void* handle;
    };

    /// @brief The size of a window
    /// @note Size can be logical which is defined by the system, or physical which
    /// holds the actual amount of pixels on the screen the window uses
    struct WindowSize {
        int32_t width;
        int32_t height;
    };

    /// @brief A container that holds content and can be interacted with
    /// @note This is how games display objects on screen and receive input for actions like moving
    class Window {
    public:
        /// @brief Polls the platform's windowing system to update all available windows
        /// used by this application
        static void Update();

        explicit Window();
        ~Window();

        /// @brief Sets the closing flag
        /// @param closing Whether this window should be closed
        void SetClosing(const bool closing) { mClosing = closing; }

        /// @brief Returns the logical size defined by the system
        /// @return Window logical size
        /// @note Doesn't represent the actual amount of pixels for this window, use GetPhysicalSize for that
        /// @see Window::GetPhysicalSize
        [[nodiscard]] WindowSize GetSize();

        /// @brief Returns the size which defines how many pixels this window actually uses
        /// @return Window physical size
        /// @note Doesn't represent the logical size defined by the system for this window, use GetSize for that
        /// @see Window::GetSize
        [[nodiscard]] WindowSize GetPhysicalSize();

        /// @brief Checks if this window has requested to close
        /// @return True if the window wants to close, or false if it doesn't
        [[nodiscard]] bool IsClosing() const { return mClosing; }

        /// @brief Gets the window's underlying platform handle
        /// @return Window handle
        [[nodiscard]] WindowHandle GetHandle();

    private:
        struct Impl;
        std::unique_ptr<Impl> mImpl;
        bool mClosing;
    };
} // namespace Artus::Core

#endif // ARTUS_PLATFORM_H
