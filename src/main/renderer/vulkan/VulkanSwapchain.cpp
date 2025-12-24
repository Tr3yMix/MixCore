

#include "VulkanSwapchain.h"

#include <algorithm>

#include "util/Logger.h"

namespace MixCore::renderer::vulkan {


    void VulkanSwapchain::init(
        const VkPhysicalDevice physicalDevice,
        const VkDevice device,
        const VkSurfaceKHR surface,
        const uint32_t width,
        const uint32_t height,
        const uint32_t graphicsQueueFamily,
        const uint32_t presentQueueFamily) {

        const auto [capabilities, formats, presentModes] = querySupport(physicalDevice, surface);
        auto [format, colorSpace] = chooseSwapSurfaceFormat(formats);
        const VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes);
        m_extent = chooseSwapExtent(capabilities, width, height);
        m_imageFormat = format;

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = format;
        createInfo.imageColorSpace = colorSpace;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const uint32_t queueFamilyIndices[] = {graphicsQueueFamily, presentQueueFamily};
        if (graphicsQueueFamily != presentQueueFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain); result != VK_SUCCESS) {
            log(Logger::LogType::Error, "vkCreateSwapchainKHR failed! with code: ", result);
            throw std::runtime_error("Failed to create swapchain!");
        }
        uint32_t swapCount = 0;
        vkGetSwapchainImagesKHR(device, m_swapchain, &swapCount, nullptr);
        m_images.resize(swapCount);
        vkGetSwapchainImagesKHR(device, m_swapchain, &swapCount, m_images.data());

        m_imageViews.resize(m_images.size());
        for (size_t i = 0; i < m_images.size(); i++) {
            VkImageViewCreateInfo viewCreateInfo{};
            viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewCreateInfo.image = m_images[i];
            viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewCreateInfo.format = m_imageFormat;
            viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = 1;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;

            if (VkResult result = vkCreateImageView(device, &viewCreateInfo, nullptr, &m_imageViews[i]); result != VK_SUCCESS) {
                log(Logger::LogType::Error, "vkCreateImageView failed! with code: ", result);
                throw std::runtime_error("Failed to create swapchain image view!");
            }
        }

    }

    void VulkanSwapchain::cleanup(const VkDevice device) const {
        for (const auto view : m_imageViews) {
            vkDestroyImageView(device, view, nullptr);
        }
        vkDestroySwapchainKHR(device, m_swapchain, nullptr);
    }


    SwapchainSupportDetails VulkanSwapchain::querySupport(const VkPhysicalDevice device, const VkSurfaceKHR surface) {
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());

        return details;

    }

    VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) {

        if (availableSurfaceFormats.empty()) throw std::runtime_error("No supported surface formats!");

        for (const auto& availableFormat : availableSurfaceFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableSurfaceFormats[0];
    }

    VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {

        if (availablePresentModes.empty()) throw std::runtime_error("No supported present modes!");

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;//triple buffering
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;//guaranteed
    }

    VkExtent2D VulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const uint32_t width, const uint32_t height) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        VkExtent2D actualExtent;
        actualExtent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }

}
