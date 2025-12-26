
#include "Window.h"

namespace MixCore::linux {

    Window::Window(const std::string& title, const math::Vector2u& windowSize) : m_width(windowSize.x), m_height(windowSize.y) {

        m_display = XOpenDisplay(nullptr);
        if (!m_display) {
            throw std::runtime_error("Failed to open X11 display");
        }

        const int screen = DefaultScreen(m_display);

        m_window = XCreateSimpleWindow(
            m_display,
            RootWindow(m_display, screen),
            0, 0,
            m_width.signedInt(), m_height.signedInt(),
            0,
            BlackPixel(m_display, screen),
            WhitePixel(m_display, screen)
            );

        if (!m_window) {
            throw std::runtime_error("Failed to create X11 window");
        }

        XStoreName(m_display, m_window, title.c_str());

        XSelectInput(
            m_display,
            m_window,
            ExposureMask |
            KeyPressMask |
            KeyReleaseMask |
            ButtonPressMask |
            ButtonReleaseMask |
            PointerMotionMask |
            StructureNotifyMask
            );

        m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);

        XMapWindow(m_display, m_window);

    }

    Window::~Window() {
        cleanup();
    }

    void Window::processMessages() {
        while (XPending(m_display)) {
            XEvent event;
            XNextEvent(m_display, &event);

            switch (event.type) {
                case ClientMessage:
                    if (static_cast<Atom>(event.xclient.data.l[0]) == m_wmDeleteMessage) {
                        m_isRunning = false;
                    }
                    break;

                case DestroyNotify:
                    m_isRunning = false;
                    break;
                case ConfigureNotify:
                    m_width = event.xconfigure.width;
                    m_height = event.xconfigure.height;
                    break;
                default:
                    break;
            }
        }
    }




    void Window::cleanup() {
        if (m_window) {
            XDestroyWindow(m_display, m_window);
            m_window = 0;
        }

        if (m_display) {
            XCloseDisplay(m_display);
            m_display = nullptr;
        }
    }



}
