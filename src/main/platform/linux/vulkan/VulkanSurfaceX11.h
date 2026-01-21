#pragma once

#ifdef __linux__


#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

#include "platform/PlatformWindow.h"
#include "util/Logger.h"

namespace Coreful::linux {

    class VulkanSurfaceX11 final : public renderer::vulkan::VulkanSurface {
    public:
        explicit VulkanSurfaceX11(const PlatformWindow& window) : m_window(window) {}

        VkSurfaceKHR create(VkInstance& instance) override {
            Logger::log(Logger::LogType::Debug, "Creating Vulkan Surface");
            VkXlibSurfaceCreateInfoKHR info{};
            info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            info.pNext = nullptr;
            info.flags = 0;
            info.dpy = static_cast<Display*>(m_window.getInstanceHandle());
            info.window = reinterpret_cast<uintptr_t>(m_window.getNativeHandle());

            Logger::log(Logger::LogType::Debug, "Created Vulkan Surface Info");

            VkSurfaceKHR surface = VK_NULL_HANDLE;

            if (vkCreateXlibSurfaceKHR(instance, &info, nullptr, &surface) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create X11 Vulkan Surface");
            }

            Logger::log(Logger::LogType::Debug, "Created Vulkan Surface");
            return surface;
        }


    private:

        const PlatformWindow& m_window;
    };

}

#endif
