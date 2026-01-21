#include "AppWindow.h"

#ifdef _WIN32
    #include "platform/windows/Window.h"
#elif defined(__linux__)

    #include "platform/linux/Window.h"
#endif

#include "ui/Drawable.h"

namespace Coreful {
    // ReSharper disable once CppParameterMayBeConst
    AppWindow::AppWindow(const math::Vector2u& windowSize, const std::string& title){

#ifdef _WIN32
        m_platformWindow = std::make_unique<win32::Window>(
            GetModuleHandle(nullptr),
            title,
            windowSize);

#elif defined(__linux__)

        m_platformWindow = std::make_unique<linux::Window>(title, windowSize);

#endif


        m_width = windowSize.signedX();
        m_height = windowSize.signedY();
    }

    void AppWindow::draw(ui::Drawable& drawable) const {
        drawable.draw(*this);
    }

    void AppWindow::processMessages() const {
        m_platformWindow->processMessages();
    }

    bool AppWindow::isRunning() const {
        return m_platformWindow->isRunning();
    }

    PlatformWindow &AppWindow::get() const {
        return *m_platformWindow;
    }
}

