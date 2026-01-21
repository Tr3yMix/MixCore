#pragma once
#include "platform/PlatformWindow.h"

namespace Coreful{

    enum class RendererType { VULKAN, OPENGL, USER_PREFERENCE };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void init(PlatformWindow& window) = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;
    };
}
