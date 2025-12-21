#pragma once

#include "Window.h"

namespace windows {
    class EventHandler {

    public:
        static LRESULT handleMessage(const WMC& ctx);

    private:
        static void handleClose(const WMC& ctx);

        static void handleResize(const WMC& ctx);

        static void handleLeftMouseDown(const WMC& ctx);

    };
}


