
#include "VulkanRenderer.h"

#include <cstring>
#include <set>
#include <vector>

#include "util/Logger.h"

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

namespace Coreful::renderer::vulkan {

    void VulkanRenderer::init(PlatformWindow& window){
        m_window = &window;
        createInstance();
        createSurface(window);
        pickPhysicalDevice();
        createLogicalDevice();
        createSyncObjects();
        initializeSwapchain(window);
        createRenderPass();
        createGraphicsPipeline();
        createFramebuffers();
        createCommandPool();
        createCommandBuffers();

        for (size_t i = 0; i < m_commandBuffers.size(); i++) recordCommandBuffers(m_commandBuffers[i], static_cast<uint32_t>(i));

        log(Logger::LogType::Info, "Vulkan Initialized!");
    }

    void VulkanRenderer::createInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Coreful";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Coreful Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;

        const std::vector extensions = VulkanPlatform::requiredInstanceExtensions();


        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> available(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, available.data());

        if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        if (ENABLE_VALIDATION_LAYERS) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
            log(Logger::LogType::Info, "Validation Layers Enabled!");
        }else createInfo.enabledLayerCount = 0;


        if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance); result != VK_SUCCESS) {
            log(Logger::LogType::Error, "Failed to create Vulkan Instance! Error code: ", result);
            throw std::runtime_error("Failed to create Vulkan Instance!");
        }
        log(Logger::LogType::Info, "Vulkan Instance Created!");
    }

    void VulkanRenderer::createSurface(const PlatformWindow& window){
        log(Logger::LogType::Info, "Creating Surface...");
        const std::unique_ptr<VulkanSurface> surfaceFactory = VulkanPlatform::getSurface(window);
        m_surface = surfaceFactory->create(m_instance);
    }


    void VulkanRenderer::pickPhysicalDevice() {
        log(Logger::LogType::Info, "Picking Physical Device...");
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
        log(Logger::LogType::Info, "Creating Logical Device...");
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

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;


        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        bool maintenance1Supported = false;
        for (const auto& extension : availableExtensions) {
            if (strcmp(extension.extensionName, VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME) == 0) {
                maintenance1Supported = true;
                break;
            }
        }

        VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT swapchainMaintenance1Features{};
        swapchainMaintenance1Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_EXT;
        swapchainMaintenance1Features.pNext = nullptr;
        swapchainMaintenance1Features.swapchainMaintenance1 = VK_TRUE;


        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.pNext = maintenance1Supported ? &swapchainMaintenance1Features : nullptr;

        std::vector deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        if (maintenance1Supported) {deviceExtensions.push_back(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME);}

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();


        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }

        uint32_t enabledCount = 0;
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &enabledCount, nullptr);
        std::vector<VkExtensionProperties> enabledExtensions(enabledCount);
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &enabledCount, enabledExtensions.data());

        m_hasSwapchainMaintenance1 = false;
        for (const auto& extension : enabledExtensions) {
            if (strcmp(extension.extensionName, VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME) == 0) {
                m_hasSwapchainMaintenance1 = true;
                break;
            }
        }

        vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
    }

    void VulkanRenderer::createSyncObjects() {

        m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create synchronization objects for a frame!");
                }
        }

        // --- Presentation fences (maintenance1) ---
        if (m_hasSwapchainMaintenance1) {
            m_presentFences.resize(MAX_FRAMES_IN_FLIGHT);

            VkFenceCreateInfo presentFenceInfo{};
            presentFenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            presentFenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                if (vkCreateFence(m_device, &presentFenceInfo, nullptr, &m_presentFences[i]) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create presentation fence!");
                }
            }
        }
    }

    void VulkanRenderer::initializeSwapchain(const PlatformWindow& window) {
        log(Logger::LogType::Info, "Initializing Swapchain...");

        if (const SwapchainSupportDetails support = VulkanSwapchain::querySupport(m_physicalDevice, m_surface);
            support.formats.empty() || support.presentModes.empty()) {
            throw std::runtime_error("Failed to find suitable swapchain format!");
        }

        m_swapchain.init(
            m_physicalDevice,
            m_device,
            m_surface,
            window.getWidth().raw(),
            window.getHeight().raw(),
            m_queueFamilyIndices.graphicsFamily.value(),
            m_queueFamilyIndices.presentFamily.value()
            );

        m_imagesInFlight.clear();
        m_imagesInFlight.resize(m_swapchain.getImageCount(), VK_NULL_HANDLE);

        m_renderFinishedSemaphoresPerImage.resize(m_swapchain.getImageCount());
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        for(auto& semaphore : m_renderFinishedSemaphoresPerImage) {
            if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create per-image render finished semaphore!");
            }
        }

        log(Logger::LogType::Debug, "Swapchain Initialized!");
    }

    void VulkanRenderer::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_swapchain.getImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;//TODO: add MSAA
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//clear at the start
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;//store result for presentation
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //ready for presentation

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;//index for attachment
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; //before render pass
        dependency.dstSubpass = 0; // our subpass
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass!");
        }

        log(Logger::LogType::Debug, "Render Pass Created!");

    }

    void VulkanRenderer::createDepthResources() {
        //nothing yet
        //TODO: create depth image + views
    }

    void VulkanRenderer::createFramebuffers() {

        m_framebuffers.resize(m_swapchain.getImageViews().size());

        for (size_t i = 0; i < m_swapchain.getImageViews().size(); i++) {
            const VkImageView attachments[] = {m_swapchain.getImageViews()[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_swapchain.getExtent().width;
            framebufferInfo.height = m_swapchain.getExtent().height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }

        log(Logger::LogType::Debug, "Framebuffers Created!");

    }

    void VulkanRenderer::createCommandPool() {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = m_queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }

        log(Logger::LogType::Debug, "Command Pool Created!");
    }

    void VulkanRenderer::createCommandBuffers() {
        m_commandBuffers.resize(m_framebuffers.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        log(Logger::LogType::Debug, "Command Buffers Created!");
    }

    void VulkanRenderer::recordCommandBuffers(VkCommandBuffer commandBuffer, uint32_t imageIndex) const {

        vkResetCommandBuffer(commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapchain.getExtent();

        VkClearValue clearColor{};
        clearColor.color = {0.2f, 0.3f, 0.3f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = static_cast<float>(m_swapchain.getExtent().height);
        viewport.width = static_cast<float>(m_swapchain.getExtent().width);
        viewport.height = -static_cast<float>(m_swapchain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        const VkRect2D scissor{{0, 0}, m_swapchain.getExtent()};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0); //draws a single triangle

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }

    }

    VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char> &code) const {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    void VulkanRenderer::createGraphicsPipeline() {

        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0; // Optional

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swapchain.getExtent().width);
        viewport.height = static_cast<float>(m_swapchain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{{0, 0}, m_swapchain.getExtent()};

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        std::vector dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
        dynamicState.pDynamicStates = dynamicStateEnables.data();

        //Pipeline Layout

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional

        if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
        vkDestroyShaderModule(m_device, fragShaderModule, nullptr);

    }




    void VulkanRenderer::render() {

        //Wait for this frame's in-flight fence
        vkWaitForFences(m_device,1,&m_inFlightFences[m_currentFrame],VK_TRUE,UINT64_MAX);
        vkResetFences(m_device,1,&m_inFlightFences[m_currentFrame]);

        //Acquire next image
        uint32_t imageIndex;
        const VkResult acquireResult = vkAcquireNextImageKHR(
            m_device,m_swapchain.get(),UINT64_MAX,
            m_imageAvailableSemaphores[m_currentFrame],VK_NULL_HANDLE, &imageIndex);

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) { recreateSwapchain(); return; }
        if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }


        //Wait for the fence tied to this image, if it exists
        if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(m_device, 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

        //Submit draw commands


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

        //--- Signal semaphores ---
        VkSemaphore signalSemaphore;
        if (m_hasSwapchainMaintenance1) {
            signalSemaphore = m_renderFinishedSemaphores[m_currentFrame];
        }else {
            signalSemaphore = m_renderFinishedSemaphoresPerImage[imageIndex];
        }

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphore;

        if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        // Present
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &signalSemaphore;
        const VkSwapchainKHR swapChains[] = {m_swapchain.get()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;


        //Only include a present fence if maintenance1 feature is enabled
        if (m_hasSwapchainMaintenance1) {
            VkSwapchainPresentFenceInfoKHR swapchainPresentFenceInfo{};
            vkWaitForFences(m_device, 1, &m_presentFences[m_currentFrame], VK_TRUE, UINT64_MAX);
            vkResetFences(m_device, 1, &m_presentFences[m_currentFrame]);
            swapchainPresentFenceInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_FENCE_INFO_KHR;
            swapchainPresentFenceInfo.swapchainCount = 1;
            swapchainPresentFenceInfo.pFences = &m_presentFences[m_currentFrame];
            presentInfo.pNext = &swapchainPresentFenceInfo;
        }

        if (const VkResult presentResult = vkQueuePresentKHR(m_presentQueue, &presentInfo); presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
            recreateSwapchain();
        } else if (presentResult != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swapchain image!");
        }

        m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    }



    void VulkanRenderer::cleanup() {

        if (m_hasSwapchainMaintenance1) {
            for (const auto fence : m_presentFences) {
                vkDestroyFence(m_device, fence, nullptr);
            }
        }

        vkDeviceWaitIdle(m_device);

        for (const auto framebuffer : m_framebuffers) {
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);
        }
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
        vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

        for (const auto semaphore : m_renderFinishedSemaphoresPerImage) {
            vkDestroySemaphore(m_device, semaphore, nullptr);
        }

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
        }
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

        if (m_device != VK_NULL_HANDLE) vkDestroyDevice(m_device, nullptr);
        if (m_surface != VK_NULL_HANDLE) vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        if (m_instance != VK_NULL_HANDLE) vkDestroyInstance(m_instance, nullptr);

    }


    void VulkanRenderer::recreateSwapchain() {
        uint32_t width = 0, height = 0;

        while (width == 0 || height == 0) {
            width = m_window->getWidth().raw();
            height = m_window->getHeight().raw();

            m_window->processMessages();

        }

        vkDeviceWaitIdle(m_device);

        cleanupFramebuffers();
        cleanupDepthResources();
        m_swapchain.cleanup(m_device);

        m_swapchain.init(
            m_physicalDevice,
            m_device,
            m_surface,
            width,
            height,
            m_queueFamilyIndices.graphicsFamily.value(),
            m_queueFamilyIndices.presentFamily.value()
            );

        createDepthResources();
        createFramebuffers();

        vkFreeCommandBuffers(m_device, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

        createCommandBuffers();

        for (size_t i = 0; i < m_commandBuffers.size(); i++) {
            recordCommandBuffers(m_commandBuffers[i], static_cast<uint32_t>(i));
        }

        m_imagesInFlight.clear();
        m_imagesInFlight.resize(m_swapchain.getImageCount(), VK_NULL_HANDLE);

    }

    void VulkanRenderer::cleanupFramebuffers() {

        for (const auto framebuffer : m_framebuffers) {
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);
        }
        m_framebuffers.clear();

    }

    void VulkanRenderer::cleanupDepthResources() {
        //nothing yet
        //TODO: destroy depth image, memory, and views
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

            if (presentSupport) indices.presentFamily = i;

            if (indices.isComplete()) break;

        }

        return indices;

    }

    bool VulkanRenderer::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : VALIDATION_LAYERS) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) return false;
        }
        return true;
    }

    std::vector<char> VulkanRenderer::readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary); // open at end to get size

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file!");
        }

        const size_t fileSize = file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));
        file.close();

        return buffer;
    }
}