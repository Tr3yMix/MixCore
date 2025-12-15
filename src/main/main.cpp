
#include "core/AppWindow.h"
#include "ui/AppUI.h"
#include "util/Logger.h"

/*
// ReSharper disable once CppParameterMayBeConst
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

    Logger::init();

    AppWindow window({800u, 600u}, "OverlayCrafter", hInstance);

    const ui::AppUI ui(&window);

    while (window.isRunning()) {
        window.update();
        ui.draw();
    }

    return 0;
}
*/

int main() {
    Logger::init();

    AppWindow window({800u, 600u}, "OverlayCrafter");

    const ui::AppUI ui(&window);

    math::Uint32 zero = 0;

    while (window.isRunning()) {
        window.update();
        ui.draw();
    }

    return 0;
}


