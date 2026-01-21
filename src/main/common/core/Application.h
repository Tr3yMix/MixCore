#pragma once
#include <memory>

#include "AppWindow.h"
#include "math/Vector2.h"
#include "renderer/Renderer.h"


namespace Coreful {
    class Application {

    public:
        Application();
        ~Application();

        void createWindow(const math::Vector2u& windowSize, const std::string& title);

        void createRenderer(RendererType rendererType);

        void initializeRenderer() const;

        std::unique_ptr<AppWindow> m_appWindow;
        std::unique_ptr<Renderer> m_renderer;


        void run() const;

    };
}



