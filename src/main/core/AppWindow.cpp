#include "AppWindow.h"

#include "ui/Drawable.h"

//windows
#ifdef _WIN32

#include "platform/windows/NativeWindow.h"

#endif

// ReSharper disable once CppParameterMayBeConst
AppWindow::AppWindow(const math::Vector2u& windowSize, const std::string& title){

#ifdef _WIN32

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    m_nativeWindow = std::make_unique<win::NativeWindow>(hInstance, title, windowSize);

#else
    #error "Platform not Supported"
#endif

    m_width = windowSize.signedX();
    m_height = windowSize.signedY();
}

void AppWindow::draw(ui::Drawable& drawable) const {
    drawable.draw(*this);
}

void AppWindow::update() const {
    m_nativeWindow->processMessages();
    m_nativeWindow->swapBuffers();
}

bool AppWindow::isRunning() const {
    return m_nativeWindow->isRunning();
}
