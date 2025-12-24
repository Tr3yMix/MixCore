#pragma once

#include "math/Uint32.h"

namespace MixCore {
    struct PlatformWindow {

        virtual ~PlatformWindow() = default;

        virtual void processMessages() = 0;

        [[nodiscard]] virtual bool isRunning() const = 0;

        [[nodiscard]] virtual void* getNativeHandle() const = 0;
        [[nodiscard]] virtual void* getInstanceHandle() const = 0;

        [[nodiscard]] virtual math::Uint32 getWidth() const = 0;
        [[nodiscard]] virtual math::Uint32 getHeight() const = 0;
    };
}


