
#include "core/Application.h"
#include "util/Logger.h"


int main() {
    MixCore::Logger::init();

    MixCore::Application MixCoreDemo;

    MixCoreDemo.createWindow(MixCore::math::Vector2u(1280, 720), "MixCoreDemo");
    MixCoreDemo.createRenderer(MixCore::RendererType::VULKAN);
    MixCoreDemo.initializeRenderer();
    MixCoreDemo.run();

    return 0;
}


