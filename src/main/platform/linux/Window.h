#pragma once

#include <X11/Xlib.h>

#include "math/Vector2.h"
#include "platform/PlatformWindow.h"

namespace Coreful::linux {

    class Window final : public PlatformWindow{

    public:
        Window(const std::string& title, const math::Vector2u& windowSize);
        ~Window() override;

        void processMessages() override;
        [[nodiscard]] bool isRunning() const override {
            return m_isRunning;
        }

        [[nodiscard]] void* getNativeHandle() const override {
            return reinterpret_cast<void*>(m_window);
        }
        [[nodiscard]] void* getInstanceHandle() const override {
            return m_display;
        }

        [[nodiscard]] math::Uint32 getWidth() const override {
            return m_width;
        }
        [[nodiscard]] math::Uint32 getHeight() const override {
            return m_height;
        }

    private:

        Display* m_display = nullptr;
        ::Window m_window = 0;
        Atom m_wmDeleteMessage;

        math::Uint32 m_width, m_height;
        bool m_isRunning = true;

        void cleanup();

    };

}
