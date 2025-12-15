#pragma once

#include "Text.h"
#include "core/AppWindow.h"
#include "math/Vector2.h"

namespace ui {

    enum class ButtonType {Normal, Exit};

    class Button {
        Button(ButtonType type, const math::Vector2f& position, const math::Vector2u& size,
            const Text& text, Colors colors, const std::function<void()>& onClick);

        void draw(DrawTarget& target) const;

    };

} // ui

