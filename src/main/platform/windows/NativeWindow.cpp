
#include "NativeWindow.h"

#include <stdexcept>

#include "NativeEventHandler.h"

namespace win {

    // ReSharper disable once CppParameterMayBeConst
    NativeWindow::NativeWindow(HINSTANCE hinstance, const std::string& title, const math::Vector2u& windowSize):
    m_width(windowSize.x), m_height(windowSize.y), m_isRunning(true) {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC; //use opengl for device context

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

        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) throw std::runtime_error("Failed to get device context.");

        if (!m_glContext.initialize(m_hdc)) {
            throw std::runtime_error("Failed to initialize OpenGL context.");
        }
        m_glContext.makeCurrent();

        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);


    }

    NativeWindow::~NativeWindow() {
        cleanup();
    }

    // ReSharper disable once CppParameterMayBeConst
    LRESULT NativeWindow::WndProc(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        auto* instance = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (msg == WM_NCCREATE ) {
            const auto* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            instance = static_cast<NativeWindow*>(createStruct->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
        }
        if (instance) {
            return NativeEventHandler::handleMessage(WMC(hWnd, msg, wParam, lParam, instance));
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);

    }

    void NativeWindow::cleanup() {

        m_glContext.cleanup();

        if (m_hdc) {
            ReleaseDC(m_hwnd, m_hdc);
            m_hdc = nullptr;
        }
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }

    }

    void NativeWindow::processMessages() const {
        MSG msg = {};

        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void NativeWindow::swapBuffers() const {
        m_glContext.swapBuffers();
    }

    HDC NativeWindow::getDC() const {
        return m_hdc;
    }

    HWND NativeWindow::getWindowHandle() const {
        return m_hwnd;
    }


}
