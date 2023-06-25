#include <airkit/VKRender/airVkRenderDriver.h>
#include <airkit/Plat/airLoger.h>

namespace air
{

    void VkRenderDriver::check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        errlog("[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

#ifdef _air_vulkan_debug_report
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
    {
        (void)flags;
        (void)object;
        (void)location;
        (void)messageCode;
        (void)pUserData;
        (void)pLayerPrefix; // Unused arguments
        errlog("[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
        return VK_FALSE;
    }
#endif

    void *vulkan_alloc_allocation(void *user_data, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope)
    {
        // Null MUST be returned if this fails.
        if (size == 0)
        {
            return 0;
        }

        void *result = getSharedAlloctor().alloctor(size,
#ifdef _check_memory_free
                                                    this_file(), this_line()
#endif
        );

        return result;
    }

    void vulkan_alloc_free(void *user_data, void *memory)
    {
        getSharedAlloctor().dealloctor(memory);
    }

    void *vulkan_alloc_reallocation(void *user_data, void *original, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope)
    {
        if (!original)
        {
            return vulkan_alloc_allocation(user_data, size, alignment, allocation_scope);
        }

        if (size == 0)
        {
            return 0;
        }
        return nullptr;
    }

    void vulkan_alloc_internal_alloc(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
    {
    }

    void vulkan_alloc_internal_free(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
    {
    }

    void VkRenderDriver::setup()
    {
        mACallback = nullptr;
        /*
        mAllocCallback.pfnAllocation = vulkan_alloc_allocation;
        mAllocCallback.pfnFree = vulkan_alloc_free;
        mAllocCallback.pfnReallocation = vulkan_alloc_reallocation;
        mAllocCallback.pfnInternalAllocation = vulkan_alloc_internal_alloc;
        mAllocCallback.pfnInternalFree = vulkan_alloc_internal_free;
        mAllocCallback.pUserData = nullptr;
*/

        mInstance = nullptr;
        mPhysical = nullptr;
        mDevice = nullptr;
        mDescPool = nullptr;

        mGraphicsIndex = -1;
        mPresentIndex = -1;
        mComputeIndex = -1;
        mTransferIndex = -1;

        mGraphicsQueue = nullptr;
        mPresentQueue = nullptr;
        mComputeQueue = nullptr;
        mTransferQueue = nullptr;

        createInstance();
        selectGPU();
        selectQueueFamily();
        createDeveice();
        createDescriptorPool();
    }

    void VkRenderDriver::destory()
    {
        vkDestroyDescriptorPool(mDevice, mDescPool, mACallback);

#ifdef _air_vulkan_debug_report
        // Remove the debug report callback
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(mInstance, mReportCallback, mACallback);
#endif

        vkDestroyDevice(mDevice, mACallback);
        vkDestroyInstance(mInstance, mACallback);
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

        uint32 extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        mPhysExt.resize(extensions_count);
        memcpy(mPhysExt.data(), extensions, sizeof(cstring) * extensions_count);

#ifdef _air_vulkan_debug_report
        mPhysExt.pushBack("VK_EXT_debug_report");
#endif
        // 创建vulkan实例

        VkInstanceCreateInfo crtinfo = {};
        crtinfo.pApplicationInfo = &app_info;
        crtinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        crtinfo.enabledExtensionCount = mPhysExt.size();
        crtinfo.ppEnabledExtensionNames = mPhysExt.data();
#ifdef _air_vulkan_debug_report
        mLayers.pushBack("VK_LAYER_KHRONOS_validation");
        crtinfo.enabledLayerCount = mLayers.size();
        crtinfo.ppEnabledLayerNames = mLayers.data();
        err = vkCreateInstance(&crtinfo, mACallback, &mInstance);
        check_vk_result(err);

        // Get the function pointer (required for any extensions)
        auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugReportCallbackEXT");
        make_ensure(vkCreateDebugReportCallbackEXT != NULL);

        // Setup the debug report callback
        VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
        debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        debug_report_ci.pfnCallback = debug_report;
        debug_report_ci.pUserData = NULL;
        err = vkCreateDebugReportCallbackEXT(mInstance, &debug_report_ci, mACallback, &mReportCallback);
        check_vk_result(err);
#else
        err = vkCreateInstance(&crtinfo, mACallback, &mInstance);
        check_vk_result(err);

#endif
    }

    void VkRenderDriver::selectGPU()
    {
        VkResult err;
        uint32 gpucnt = 0;
        err = vkEnumeratePhysicalDevices(mInstance, &gpucnt, NULL);
        check_vk_result(err);
        mPhysicalset.resize(gpucnt);
        err = vkEnumeratePhysicalDevices(mInstance, &gpucnt, mPhysicalset.data());
        check_vk_result(err);

        uint32 gpu = 0; // 选择的GPU
        for (uint32 i = 0; i < gpucnt; i++)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(mPhysicalset[i], &properties);
            // 最好是独立GPU而非集成GPU
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                gpu = i;
                break;
            }
        }
        // 记录使用的物理设备
        mPhysical = mPhysicalset[gpu];
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
        err = vkCreateDevice(mPhysical, &create_info, mACallback, &mDevice);
        check_vk_result(err);
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
        err = vkCreateDescriptorPool(mDevice, &pool_info, mACallback, &mDescPool);
        check_vk_result(err);
    }
}