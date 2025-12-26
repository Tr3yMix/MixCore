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
        VkRenderPass m_renderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> m_framebuffers;
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_commandBuffers;

        VkSemaphore m_imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_renderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_inFlightFence = VK_NULL_HANDLE;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

        //INITIALIZATION FUNCTIONS
        void createInstance();
        void createSurface(const PlatformWindow& window);
        void pickPhysicalDevice();//locate and select gpu for vulkan
        void createLogicalDevice();
        void createSyncObjects();
        void initializeSwapchain(const PlatformWindow& window);
        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void recordCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;
        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        void createGraphicsPipeline();

        static std::vector<char> readFile(const std::string& filename);
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);



    };
}

