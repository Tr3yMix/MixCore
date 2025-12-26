#include "VulkanPlatform.h"

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
    #include "platform/windows/vulkan/VulkanSurfaceWin32.h"
#elif defined(__linux__)
    #define VK_USE_PLATFORM_XLIB_KHR
    #include "platform/linux/vulkan/VulkanSurfaceX11.h"
#endif

namespace MixCore::renderer::vulkan {
    std::vector<const char*> VulkanPlatform::requiredInstanceExtensions() {
        {
            std::vector extensions = {
                VK_KHR_SURFACE_EXTENSION_NAME
            };

#ifdef _WIN32
            extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif __linux__
            extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
            extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

#endif

            return extensions;
        }
    }

    std::unique_ptr<VulkanSurface> VulkanPlatform::getSurface(const PlatformWindow &window) {
#ifdef _WIN32
        return std::make_unique<win32::VulkanSurfaceWin32>(window);
#elif defined(__linux__)
        return std::make_unique<linux::VulkanSurfaceX11>(window);
#else
        static_assert(false, "Unsupported platform")
#endif
    }
}





