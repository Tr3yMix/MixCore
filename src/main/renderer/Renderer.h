#pragma once
#include "platform/PlatformWindow.h"

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void init(const PlatformWindow& window) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
};
