
#include "core/Application.h"
#include "util/Logger.h"


int main() {

    Coreful::Logger::init();

    Coreful::Application CorefulApp;

    CorefulApp.createWindow(Coreful::math::Vector2u(1080, 720), "CorefulAppDemo");

    CorefulApp.createRenderer(Coreful::RendererType::VULKAN);//TODO: Add support for pluggable renderer backends selectable at runtime

    CorefulApp.initializeRenderer();

    CorefulApp.run();

    return 0;
}


