//
// Created by fightinghawks18 on 2/1/2026.
//

#include "Artus/Core/Window.h"

#include "Artus/Core/Logger.h"

#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Artus::Core::Window* window = nullptr;
    if (msg == WM_NCCREATE) {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        window = static_cast<Artus::Core::Window*>(createStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Artus::Core::Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    switch (msg) {
    case WM_CLOSE: {
        window->SetClosing(true);
        return 0;
    }
    default: break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

namespace Artus::Core {
    struct Window::Impl {
        HWND hWnd;
    };

    void Window::Update() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Window::Window() : mImpl(std::make_unique<Impl>()), mClosing(false) {
        WNDCLASSEX wc = {0};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = &WndProc;
        wc.lpszClassName = "ArtusWindow";

        if (!RegisterClassEx(&wc)) {
            AR_ERR("Failed to successfully register a win32 window class!");
            return;
        }

        mImpl->hWnd = CreateWindowEx(
            0,
            "ArtusWindow",
            "Artus",
            WS_OVERLAPPEDWINDOW,
            0,
            0,
            800,
            600,
            nullptr,
            nullptr,
            nullptr,
            this);
        if (!mImpl->hWnd) {
            AR_ERR("Failed to successfully create a win32 window!");
            return;
        }

        ShowWindow(mImpl->hWnd, SW_SHOW);
    }

    Window::~Window() {
        if (!IsWindow(mImpl->hWnd))
            return;
        DestroyWindow(mImpl->hWnd);
        mImpl->hWnd = nullptr;
    }

    WindowSize Window::GetSize() const {
        RECT rect;
        GetWindowRect(mImpl->hWnd, &rect);
        return {rect.right-rect.left, rect.bottom-rect.top};
    }

    WindowSize Window::GetPhysicalSize() const {
        RECT rect;
        GetClientRect(mImpl->hWnd, &rect);
        return {rect.right-rect.left, rect.bottom-rect.top};
    }

    WindowHandle Window::GetHandle() const { return {mImpl->hWnd}; }
} // namespace Artus::Core