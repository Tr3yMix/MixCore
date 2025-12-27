
#include "core/Application.h"
#include "util/Logger.h"


int main() {

    MixCore::Logger::init();

    MixCore::Application MixCoreDemo;

    MixCoreDemo.createWindow(MixCore::math::Vector2u(1280, 720), "MixCoreDemo");

    MixCoreDemo.createRenderer(MixCore::RendererType::VULKAN);//TODO: Add support for pluggable renderer backends selectable at runtime

    MixCoreDemo.initializeRenderer();

    MixCoreDemo.run();

    return 0;
}


