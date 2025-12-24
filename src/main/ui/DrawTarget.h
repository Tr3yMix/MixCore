#pragma once

namespace MixCore::ui {
    class Drawable;

    class DrawTarget {

    public:
        virtual ~DrawTarget() = default;

        [[nodiscard]] unsigned int getWidth() const {return m_width;}
        [[nodiscard]] unsigned int getHeight() const {return m_height;}

        int m_width = 0, m_height = 0;

        virtual void draw(Drawable& drawable) const = 0;
    };
}
