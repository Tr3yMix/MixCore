#pragma once

#include "renderer/Renderer.h"

#include "platform/PlatformWindow.h"

#include "renderer/VulkanPlatform.h"

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

    void createInstance();
    void pickPhysicalDevice();//locate and select gpu for vulkan
    void createLogicalDevice();
    void createSurface(const PlatformWindow& window);


};

