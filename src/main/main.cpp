
#include "core/Application.h"
#include "core/AppWindow.h"
#include "platform/windows/Window.h"
#include "platform/windows/vulkan/VulkanSurfaceWin32.h"
#include "ui/AppUI.h"
#include "util/Logger.h"


int main() {
    Logger::init();

    Application MixCoreDemo;

    MixCoreDemo.createWindow(math::Vector2u(1280, 720), "MixCoreDemo");
    MixCoreDemo.createRenderer();
    MixCoreDemo.initializeRenderer();
    MixCoreDemo.run();


    return 0;
}


