#pragma once


#include <memory>
#include <vector>

#include "VulkanSurface.h"
#include "platform/PlatformWindow.h"


namespace MixCore::renderer::vulkan {

    class VulkanPlatform {
    public:
        static std::vector<const char*> requiredInstanceExtensions();
        static std::unique_ptr<VulkanSurface> getSurface(const PlatformWindow& window);
    };


}
