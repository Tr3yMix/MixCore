#pragma once
#include <vulkan/vulkan_core.h>

namespace MixCore::renderer::vulkan {
    class VulkanSurface {
    public:
        virtual ~VulkanSurface() = default;
        virtual VkSurfaceKHR create(VkInstance& instance) = 0;
    };
}
