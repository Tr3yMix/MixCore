#pragma once
#include <vulkan/vulkan_core.h>

namespace Coreful::renderer::vulkan {
    class VulkanSurface {
    public:
        virtual ~VulkanSurface() = default;
        virtual VkSurfaceKHR create(VkInstance& instance) = 0;
    };
}
