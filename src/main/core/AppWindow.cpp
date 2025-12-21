#include "AppWindow.h"

#ifdef _WIN32
#include "platform/windows/Window.h"
#endif

#include "ui/Drawable.h"

// ReSharper disable once CppParameterMayBeConst
AppWindow::AppWindow(const math::Vector2u& windowSize, const std::string& title){

#ifdef _WIN32
    m_platformWindow = std::make_unique<windows::Window>(
        GetModuleHandle(nullptr),
        "MixCore",
        math::Vector2u(1280, 720));
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

