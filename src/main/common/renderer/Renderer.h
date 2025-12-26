#pragma once
#include "platform/PlatformWindow.h"

namespace MixCore{

    enum class RendererType { VULKAN, OPENGL, DIRECTX11, DIRECTX12 };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void init(const PlatformWindow& window) = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;
    };
}
