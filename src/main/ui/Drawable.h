#pragma once

namespace MixCore::ui {
    class DrawTarget;

    class Drawable {

    public:
        virtual ~Drawable() = default;

        virtual void draw(const DrawTarget& target) const = 0;
    };
}
