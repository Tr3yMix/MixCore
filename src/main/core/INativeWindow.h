#pragma once


#include "Event.h"
#include "math/Uint32.h"

class INativeWindow {
    public:
        virtual ~INativeWindow() = default;

        virtual void processMessages() const = 0;
        virtual void swapBuffers() const = 0;

        virtual void pollEvent(Event& event) = 0;

        [[nodiscard]] virtual math::Uint32 getWidth() const = 0;
        [[nodiscard]] virtual math::Uint32 getHeight() const = 0;
        [[nodiscard]] virtual bool isRunning() const = 0;

};
