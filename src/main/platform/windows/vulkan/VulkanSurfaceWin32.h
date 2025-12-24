#pragma once

#include <windows.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>


#include "platform/PlatformWindow.h"

#include "renderer/vulkan/VulkanSurface.h"

namespace MixCore::win32 {
    class VulkanSurfaceWin32 final : public renderer::vulkan::VulkanSurface {
    public:
        explicit VulkanSurfaceWin32(const PlatformWindow& window) : m_window(window) {}

        VkSurfaceKHR create(VkInstance& instance) override {
            VkWin32SurfaceCreateInfoKHR info{};
            info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            info.hwnd = static_cast<HWND>(m_window.getNativeHandle());
            info.hinstance = static_cast<HINSTANCE>(m_window.getInstanceHandle());


            VkSurfaceKHR surface;

            if (vkCreateWin32SurfaceKHR(instance, &info, nullptr, &surface) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create Win32 Vulkan Surface");
            }

            return surface;
        }


    private:
        const PlatformWindow& m_window;
    };
}
