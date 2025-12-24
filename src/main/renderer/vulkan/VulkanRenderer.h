#pragma once

#include "VulkanQueues.h"
#include "VulkanSwapchain.h"
#include "renderer/Renderer.h"

#include "platform/PlatformWindow.h"

#include "VulkanPlatform.h"

namespace MixCore::renderer::vulkan {
    class VulkanRenderer final : public Renderer {

    public:

        void init(const PlatformWindow& window) override;
        void render() override;
        void cleanup() override;

    private:

        VkInstance m_instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;//the gpu
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;
        QueueFamilyIndices m_queueFamilyIndices;
        VulkanSwapchain m_swapchain;

        void createInstance();
        void createSurface(const PlatformWindow& window);
        void pickPhysicalDevice();//locate and select gpu for vulkan
        void createLogicalDevice();
        void initializeSwapchain(const PlatformWindow& window);

        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);



    };
}

