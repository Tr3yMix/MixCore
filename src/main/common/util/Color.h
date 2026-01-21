#pragma once
#include <cstdint>

namespace Coreful::util {


    class Color {

        public:

        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

        explicit Color(std::uint32_t hex);

        [[nodiscard]] std::uint8_t r() const {return m_r;}
        [[nodiscard]] std::uint8_t g() const {return m_g;}
        [[nodiscard]] std::uint8_t b() const {return m_b;}
        [[nodiscard]] std::uint8_t a() const {return m_a;}

        [[nodiscard]] float rF() const {return static_cast<float>(m_r) / 255.f;}
        [[nodiscard]] float gF() const {return static_cast<float>(m_g) / 255.f;}
        [[nodiscard]] float bF() const {return static_cast<float>(m_b) / 255.f;}
        [[nodiscard]] float aF() const {return static_cast<float>(m_a) / 255.f;}

    private:
        std::uint8_t m_r, m_g, m_b, m_a;

        static std::uint8_t parseR(const std::uint32_t hex) {
            return hex <= 0xFFF ? (hex >> 8 & 0xF) * 17 :
            hex <= 0xFFFFFF ? hex >> 16 & 0xFF : hex >> 24 & 0xFF;
        }
        static std::uint8_t parseG(const std::uint32_t hex) {
            return hex <= 0xFFF ? (hex >> 4 & 0xF) * 17 :
            hex <= 0xFFFFFF ? hex >> 8 & 0xFF : hex >> 16 & 0xFF;
        }
        static std::uint8_t parseB(const std::uint32_t hex) {
            return hex <= 0xFFF ? (hex & 0xF) * 17 :
            hex <= 0xFFFFFF ? hex & 0xFF : hex >> 8 & 0xFF;
        }
        static std::uint8_t parseA(const std::uint32_t hex) {
            return hex <= 0xFFFFFF ? 255 : hex & 0xFF;
        }

    };

}

