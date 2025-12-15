#pragma once
#include <string>
#include <windows.h>

#include "NativeGLContext.h"
#include "core/INativeWindow.h"
#include "math/Uint32.h"
#include "math/Vector2.h"

namespace win {

    //window message context
    struct WMC {
        HWND hwnd;
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;
        class NativeWindow* window;

        // ReSharper disable once CppParameterMayBeConst
        WMC(HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam, NativeWindow* window) : hwnd(hwnd), msg(msg), wParam(wParam), lParam(lParam), window(window) {}
    };

    class NativeWindow : public INativeWindow {
    public:
        NativeWindow(HINSTANCE hinstance, const std::string& title, const math::Vector2u& windowSize);
        ~NativeWindow() override;

        [[nodiscard]] HDC getDC() const;
        [[nodiscard]] HWND getWindowHandle() const;

        [[nodiscard]] math::Uint32 getWidth() const override {return m_width;}
        [[nodiscard]] math::Uint32 getHeight() const override {return m_height;}
        [[nodiscard]] bool isRunning() const override {return m_isRunning;}

        void processMessages() const override;
        void swapBuffers() const override;

        math::Uint32 m_width, m_height;

        bool m_isRunning;

    private:

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        NativeGLContext m_glContext;

        HWND m_hwnd;
        HDC m_hdc;
        HGLRC m_hglrc{};

        void cleanup();

    };

} // win
