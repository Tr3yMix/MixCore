
#include "Application.h"

#include "renderer/vulkan/VulkanRenderer.h"


Application::Application(){
}

Application::~Application() {
}

void Application::createWindow(const math::Vector2u &windowSize, const std::string &title) {
    m_appWindow = std::make_unique<AppWindow>(windowSize, title);
}

void Application::createRenderer() {
    m_renderer = std::make_unique<VulkanRenderer>();
}

void Application::initializeRenderer() const {
    if (!m_appWindow) {
        throw std::runtime_error("Application Window not created!");
    }
    if (!m_renderer) {
        throw std::runtime_error("Renderer not created!");
    }
    m_renderer->init(m_appWindow->get());
}

void Application::run() const {
    while (m_appWindow->isRunning()) {
        m_appWindow->processMessages();
        m_renderer->render();
    }
}

