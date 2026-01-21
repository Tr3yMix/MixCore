#pragma once

#include <memory>

#include "math/Vector2.h"
#include "platform/PlatformWindow.h"
#include "ui/DrawTarget.h"

namespace Coreful {
    class AppWindow final : public ui::DrawTarget{

    public:

        AppWindow(const math::Vector2u& windowSize, const std::string& title);

        void draw(ui::Drawable& drawable) const override;

        std::unique_ptr<PlatformWindow> m_platformWindow;

        void processMessages() const;

        [[nodiscard]] bool isRunning() const;

        [[nodiscard]] PlatformWindow& get() const;


    };
}



