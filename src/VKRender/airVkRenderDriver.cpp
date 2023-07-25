#include <airkit/VKRender/airVkRenderDriver.h>
#include <airkit/Plat/airLoger.h>

namespace air
{
    void VkRenderDriver::checkVKResult(VkResult err)
    {
        if (err != 0)
            errlog("[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VkRenderDriver::debugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
                                                               uint64_t object, size_t location, int32_t messageCode,
                                                               const char *pLayerPrefix, const char *pMessage, void *pUserData)
    {
        errlog("[vulkan] Debug report from ObjectType: %i\n\tMessage: %s\n\n", objectType, pMessage);
        return VK_FALSE;
    }

    VkRenderDriver::VkRenderDriver()
    {
        mAllocCall = nullptr;
        mInstance = nullptr;
        mDebugReport = nullptr;

        mPhysical = nullptr;

        mGraphicsIndex = -1;
        mComputeIndex = -1;
        mTransferIndex = -1;

        mGraphicsQueue = nullptr;
        mComputeQueue = nullptr;
        mTransferQueue = nullptr;

        mDevice = nullptr;
        mDescPool = nullptr;

        
    }

    void VkRenderDriver::setup()
    {
        createInstance();
        selectGPU();
        selectQueueFamily();
        createDeveice();
        createDescriptorPool();
    }

    void VkRenderDriver::destory()
    {
        vkDestroyDescriptorPool(mDevice, mDescPool, mAllocCall);

#ifdef _air_vulkan_debug_report
        // Remove the debug report callback
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(mInstance, mDebugReport, mAllocCall);
#endif

        vkDestroyDevice(mDevice, mAllocCall);
        vkDestroyInstance(mInstance, mAllocCall);
    }

    void VkRenderDriver::checkLayerSupport()
    {
        uint32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        Vector<VkLayerProperties> availableLayers;
        availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    }
    void VkRenderDriver::createInstance()
    {
        VkResult err;

        VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
        app_info.apiVersion = VK_API_VERSION_1_2;
        app_info.pApplicationName = "airkit-ui-vk";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "airkit-engine-vk";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        // 获取实例扩展信息
        uint32 extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        mInsExt.resize(extensions_count);
        memcpy(mInsExt.data(), extensions, sizeof(cstring) * extensions_count);

#ifdef _air_vulkan_debug_report
        mInsExt.pushBack(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        checkLayerSupport();
#endif
        // 创建vulkan实例

        VkInstanceCreateInfo crtinfo = {};
        crtinfo.pApplicationInfo = &app_info;
        crtinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        crtinfo.enabledExtensionCount = mInsExt.size();
        crtinfo.ppEnabledExtensionNames = mInsExt.data();

        // 设置开启的验证层扩展
#ifdef _air_vulkan_debug_report
        mLayers.pushBack("VK_LAYER_KHRONOS_validation");
        crtinfo.enabledLayerCount = mLayers.size();
        crtinfo.ppEnabledLayerNames = mLayers.data();
        err = vkCreateInstance(&crtinfo, mAllocCall, &mInstance);
        checkVKResult(err);

        // 获取函数地址
        auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugReportCallbackEXT");
        make_ensure(vkCreateDebugReportCallbackEXT != NULL);

        // 创建调试信息回调
        VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
        debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        debug_report_ci.pfnCallback = debugReport;
        debug_report_ci.pUserData = NULL;
        err = vkCreateDebugReportCallbackEXT(mInstance, &debug_report_ci, mAllocCall, &mDebugReport);
        checkVKResult(err);
#else
        err = vkCreateInstance(&crtinfo, mAllocCall, &mInstance);
        checkVKResult(err);

#endif
    }

    void VkRenderDriver::selectGPU()
    {
        VkResult err;
        uint32 gpucnt = 0;
        err = vkEnumeratePhysicalDevices(mInstance, &gpucnt, NULL);
        checkVKResult(err);
        mPhysicalSet.resize(gpucnt);
        err = vkEnumeratePhysicalDevices(mInstance, &gpucnt, mPhysicalSet.data());
        checkVKResult(err);

        uint32 gpu = 0; // 选择的GPU
        for (uint32 i = 0; i < gpucnt; i++)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(mPhysicalSet[i], &properties);
            // 最好是独立GPU而非集成GPU
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                gpu = i;
                break;
            }
        }
        // 记录使用的物理设备
        mPhysical = mPhysicalSet[gpu];
    }

    void VkRenderDriver::selectQueueFamily()
    {
        VkResult err;
        uint32 count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysical, &count, NULL);
        mQueueFamilyPro.resize(count);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysical, &count, mQueueFamilyPro.data());
        uint32 idxcnt = 0;
        for (uint32 i = 0; i < count; i++)
        {
            if (mGraphicsIndex == -1 && mQueueFamilyPro[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                ++idxcnt;
                mGraphicsIndex = i;
            }
            if (mComputeIndex == -1 && mQueueFamilyPro[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                ++idxcnt;
                mComputeIndex = i;
            }
            if (mTransferIndex == -1 && mQueueFamilyPro[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                ++idxcnt;
                mTransferIndex = i;
            }
            if (idxcnt == 3)
                break;
        }
        make_ensure(mGraphicsIndex != -1);
        make_ensure(mComputeIndex != -1);
        make_ensure(mTransferIndex != -1);
    }
    void VkRenderDriver::createDeveice()
    {
        VkResult err;
        mDeviceExt.pushBack("VK_KHR_swapchain");

        const float queue_priority[] = {1.0f};
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = mGraphicsIndex;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = array_size(queue_info, VkDeviceQueueCreateInfo);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = mDeviceExt.size();
        create_info.ppEnabledExtensionNames = mDeviceExt.data();
        err = vkCreateDevice(mPhysical, &create_info, mAllocCall, &mDevice);
        checkVKResult(err);
        // 获取队列
        vkGetDeviceQueue(mDevice, mGraphicsIndex, 0, &mGraphicsQueue);
        vkGetDeviceQueue(mDevice, mComputeIndex, 0, &mComputeQueue);
        vkGetDeviceQueue(mDevice, mTransferIndex, 0, &mTransferQueue);
    }
    void VkRenderDriver::createDescriptorPool()
    {
        VkResult err;
        VkDescriptorPoolSize pool_sizes[] =
            {
                {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * array_size(pool_sizes, VkDescriptorPoolSize);
        pool_info.poolSizeCount = (uint32_t)array_size(pool_sizes, VkDescriptorPoolSize);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(mDevice, &pool_info, mAllocCall, &mDescPool);
        checkVKResult(err);
    }

  
}