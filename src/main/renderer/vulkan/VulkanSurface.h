#pragma once

class VulkanSurface {
public:
    virtual ~VulkanSurface() = default;
    virtual VkSurfaceKHR create(VkInstance& instance) = 0;
};
