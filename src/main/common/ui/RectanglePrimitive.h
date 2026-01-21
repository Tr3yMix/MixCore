#pragma once

#include "Drawable.h"
#include "math/Vector2.h"
#include "util/Color.h"

namespace Coreful::ui {
    class RectanglePrimitive final : public Drawable{

    public:

        RectanglePrimitive(math::Vector2f position, float width, float height);

        void setColor(const util::Color& color);

        void draw(const DrawTarget& target) const override;

    private:

        math::Vector2f m_position;
        float m_width, m_height;
        float m_r = 1.0f, m_g = 1.0f, m_b = 1.0f, m_a = 1.0f;

    };
}



