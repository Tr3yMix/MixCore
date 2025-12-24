
#include "VulkanRenderer.h"

#include <set>
#include <vector>

#include "util/Logger.h"

namespace MixCore::renderer::vulkan {
    void VulkanRenderer::init(const PlatformWindow& window) {
        createInstance();
        createSurface(window);
        pickPhysicalDevice();
        createLogicalDevice();
        initializeSwapchain(window);
        log(Logger::LogType::Debug, "Vulkan Initialized!");
    }

    void VulkanRenderer::createInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "MixCore";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "MixCore Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        const std::vector extensions = platform::requiredInstanceExtensions();


        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> available(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, available.data());


        if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }

    void VulkanRenderer::createSurface(const PlatformWindow& window){
        VulkanSurface* surfaceFactory = platform::getSurface(window);
        m_surface = surfaceFactory->create(m_instance);
        delete surfaceFactory;
    }


    void VulkanRenderer::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        for(const auto& device : devices) {
            if (QueueFamilyIndices indices = findQueueFamilies(device, m_surface); indices.isComplete()) {
                m_physicalDevice = device;
                m_queueFamilyIndices = indices;
                return;
            }
        }
        throw std::runtime_error("Failed to find a suitable GPU for the vulkan renderer!");
    }

    void VulkanRenderer::createLogicalDevice() {
        constexpr float queuePriority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        const std::set uniqueQueues = {
            m_queueFamilyIndices.graphicsFamily.value(),
            m_queueFamilyIndices.presentFamily.value()
        };

        for (const uint32_t family : uniqueQueues) {
            VkDeviceQueueCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.queueFamilyIndex = family;
            info.queueCount = 1;
            info.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(info);
        }

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        const std::vector deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();


        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
    }

    void VulkanRenderer::initializeSwapchain(const PlatformWindow& window) {
        m_swapchain.init(
            m_physicalDevice,
            m_device,
            m_surface,
            window.getWidth().raw(),
            window.getHeight().raw(),
            m_queueFamilyIndices.graphicsFamily.value(),
            m_queueFamilyIndices.presentFamily.value()
            );

        log(Logger::LogType::Debug, "Swapchain Initialized!");
    }



    void VulkanRenderer::render() {

        //TODO: setup rendering for Linux/Windows windows
    }

    void VulkanRenderer::cleanup() {
        if (m_device != VK_NULL_HANDLE) vkDestroyDevice(m_device, nullptr);
        if (m_surface != VK_NULL_HANDLE) vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        if (m_instance != VK_NULL_HANDLE) vkDestroyInstance(m_instance, nullptr);

    }


    // ReSharper disable once CppParameterMayBeConst
    QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (!presentSupport) throw std::runtime_error("Surface does not support presentation!");

            indices.presentFamily = i;

            if (indices.isComplete()) break;

        }

        return indices;

    }
}




