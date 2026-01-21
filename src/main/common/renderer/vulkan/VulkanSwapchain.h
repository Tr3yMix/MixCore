#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

#include "platform/PlatformWindow.h"

namespace Coreful::renderer::vulkan {

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanSwapchain {

    public:

        void init(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkSurfaceKHR surface,
            uint32_t width, uint32_t height,
            uint32_t graphicsQueueFamily,
            uint32_t presentQueueFamily
            );

        void cleanup(VkDevice device) const;
        void recreateSwapchain();

        [[nodiscard]] VkSwapchainKHR get() const {return m_swapchain;}
        [[nodiscard]] VkFormat getImageFormat() const {return m_imageFormat;}
        [[nodiscard]] uint32_t getImageCount() const {return static_cast<uint32_t>(m_images.size());}
        [[nodiscard]] VkExtent2D getExtent() const {return m_extent;}
        [[nodiscard]] const std::vector<VkImageView>& getImageViews() const {return m_imageViews;}

        static SwapchainSupportDetails querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

    private:

        VkSwapchainKHR m_swapchain{};
        VkFormat m_imageFormat{};
        VkExtent2D m_extent{};
        std::vector<VkImage> m_images;
        std::vector<VkImageView> m_imageViews;

    };

}
