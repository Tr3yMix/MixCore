#pragma once
#include <memory>

#include "renderer/Renderer.h"
#include "ui/AppUI.h"


class Application {

    public:
    Application();
    ~Application();

    void createWindow(const math::Vector2u& windowSize, const std::string& title);

    void createRenderer();

    void initializeRenderer() const;

    std::unique_ptr<AppWindow> m_appWindow;
    std::unique_ptr<Renderer> m_renderer;


    void run() const;




};

