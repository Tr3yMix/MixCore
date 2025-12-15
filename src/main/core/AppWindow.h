#pragma once

#include <memory>

#include "INativeWindow.h"
#include "math/Vector2.h"
#include "ui/DrawTarget.h"


class AppWindow final : public ui::DrawTarget{

public:

    AppWindow(const math::Vector2u& windowSize, const std::string& title);

    void draw(ui::Drawable& drawable) const override;

    std::unique_ptr<INativeWindow> m_nativeWindow;

    void update() const;

    [[nodiscard]] bool isRunning() const;


private:


};

