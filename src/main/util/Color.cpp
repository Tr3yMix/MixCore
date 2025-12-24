#include "Color.h"


namespace MixCore::util {
    Color::Color(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a): m_r(r), m_g(g), m_b(b), m_a(a) {}

    Color::Color(const std::uint32_t hex) :
    Color(parseR(hex), parseG(hex), parseB(hex), parseA(hex)){}
}
