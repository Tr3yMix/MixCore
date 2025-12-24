#pragma once

#include <string>
#include <windows.h>

#include "math/Uint32.h"
#include "math/Vector2.h"
#include "platform/PlatformWindow.h"
#include "renderer/Renderer.h"

namespace MixCore::win32 {

    //window message context
    struct WMC {
        HWND hwnd;
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;
        class Window* window;

        // ReSharper disable once CppParameterMayBeConst
        WMC(HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam, Window* window) : hwnd(hwnd), msg(msg), wParam(wParam), lParam(lParam), window(window) {}
    };

    class Window final : public PlatformWindow {
    public:
        Window(HINSTANCE hinstance, const std::string& title, const math::Vector2u& windowSize);
        ~Window() override;


        math::Uint32 m_width, m_height;

        bool m_isRunning;

        void processMessages() override;

        [[nodiscard]] bool isRunning() const override {return m_isRunning;}

        [[nodiscard]] void* getNativeHandle() const override {return m_hwnd;}
        [[nodiscard]] void* getInstanceHandle() const override {return m_hinstance;}

        [[nodiscard]] math::Uint32 getWidth() const override {return m_width;}
        [[nodiscard]] math::Uint32 getHeight() const override {return m_height;}

    private:

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        HWND m_hwnd;
        HINSTANCE m_hinstance;


        void cleanup();

    };

}
