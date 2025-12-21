#include "RectanglePrimitive.h"



#include "DrawTarget.h"
#include "math/Vector2.h"


namespace ui {
    RectanglePrimitive::RectanglePrimitive(const math::Vector2f position, const float width, const float height):
    m_position(position), m_width(width), m_height(height) {}

    void RectanglePrimitive::setColor(const util::Color& color) {
        m_r = color.rF();
        m_g = color.gF();
        m_b = color.bF();
        m_a = color.aF();
    }

    void RectanglePrimitive::draw(const DrawTarget& target) const {

    }

}






