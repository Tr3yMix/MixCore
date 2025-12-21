
#include <stdexcept>

#include "EventHandler.h"

namespace windows {

    // ReSharper disable once CppParameterMayBeConst
    Window::Window(HINSTANCE hinstance, const std::string& title, const math::Vector2u& windowSize):
    m_width(windowSize.x), m_height(windowSize.y), m_isRunning(true), m_hinstance(hinstance) {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;

        wc.lpfnWndProc = WndProc;
        wc.hInstance = hinstance;
        wc.lpszClassName = "NativeWindowClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr;
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        if (!RegisterClassExA(&wc)) throw std::runtime_error("Failed to register window class.");

        m_hwnd = CreateWindowEx(0, wc.lpszClassName, title.c_str(), WS_POPUP, CW_USEDEFAULT,
            CW_USEDEFAULT, m_width.signedInt(), m_height.signedInt(), nullptr, nullptr, hinstance, this);

        if (!m_hwnd) throw std::runtime_error("Failed to create window.");


        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);

        //m_renderer->init


    }

    Window::~Window() {
        cleanup();
    }

    void Window::processMessages() {
        MSG msg = {};

        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // ReSharper disable once CppParameterMayBeConst
    LRESULT Window::WndProc(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        auto* instance = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (msg == WM_NCCREATE ) {
            const auto* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            instance = static_cast<Window*>(createStruct->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
        }
        if (instance) {
            return EventHandler::handleMessage(WMC(hWnd, msg, wParam, lParam, instance));
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);

    }

    void Window::cleanup() {

        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }

    }


}

