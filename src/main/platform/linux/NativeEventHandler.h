#pragma once

#ifdef __linux__

#include "NativeWindow.h"

namespace win {
    class NativeEventHandler {

    public:
        static LRESULT handleMessage(const WMC& ctx);

    private:
        static void handleClose(const WMC& ctx);

        static void handleResize(const WMC& ctx);

        static void handleLeftMouseDown(const WMC& ctx);

    };
}
#endif

