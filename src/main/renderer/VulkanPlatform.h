#pragma once

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR

    #include "platform/windows/vulkan/VulkanSurfaceWin32.h"
#elif defined(__linux__)
    #define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>
#include <vector>

namespace vkplatform {
    inline std::vector<const char*> requiredInstanceExtensions() {
        std::vector extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME
        };

#ifdef _WIN32
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif __linux__
        extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif

        return extensions;
    }

    inline VulkanSurface* getSurface(const PlatformWindow& window) {
#ifdef _WIN32
        return new VulkanSurfaceWin32(window);
#elif defined(__linux__)
        return nullptr;
#endif

    }
}
