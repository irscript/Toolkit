#include <airkit/VKRender/airVkRender.h>
#include <airkit/Plat/airLoger.h>
#include <airkit/VKRender/airVkWindow.h>
#include <airkit/VKRender/airVkUIRenderPipeline.h>

namespace air
{

    void glfwErrorCallBack(int error, const char *description)
    {
    }

    bool VkRender::init()
    {
        glfwSetErrorCallback(glfwErrorCallBack);
        if (!glfwInit())
            return false;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if (!glfwVulkanSupported())
        {
            errlog("GLFW: Vulkan Not Supported!");
            return false;
        }

        mDriver.setup();
        return true;
    }
    void VkRender::terminal()
    {
        mDriver.destory();

        glfwTerminate();
    }
    IWindow *VkRender::createWindow(const String &title, flt32 width, flt32 height)
    {

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        VkWindow *window = (VkWindow *)getSharedAlloctor().alloctor(sizeof(VkWindow)
#ifdef _check_memory_free
                                                                        ,
                                                                    this_file(), this_line()
#endif //_check_memory_free
        );

        constructor<VkWindow>(window);

        window->mNative = glfwCreateWindow(width, height, title.cstr(), NULL, NULL);
        window->mWidth = width;
        window->mHeight = height;
        window->mTitle = title;

        auto err = glfwCreateWindowSurface(mDriver.mInstance, window->glfwWindow(), mDriver.mACallback, &window->mSurface);
        make_ensure(err == VkResult::VK_SUCCESS);

        setupWindow(*window);

        return window;
    }
    void VkRender::destroyWindow(IWindow *window)
    {
        auto &win = *(VkWindow *)window;
        vkDestroySwapchainKHR(mDriver.mDevice, win.mSwapchain, mDriver.mACallback);
        vkDestroySurfaceKHR(mDriver.mInstance, win.mSurface, mDriver.mACallback);

        destructor<VkWindow>(&win);
        glfwDestroyWindow(win.glfwWindow());
        getSharedAlloctor().dealloctor(window);
    }
    IUIRenderPipeline *VkRender::createUIPipeline()
    {
        VkUIPipeline *pipeline = (VkUIPipeline *)getSharedAlloctor().alloctor(sizeof(VkUIPipeline)
#ifdef _check_memory_free
                                                                                  ,
                                                                              this_file(), this_line()
#endif //_check_memory_free
        );
        constructor<VkUIPipeline>(pipeline);

        // 创建管线

        return pipeline;
    }
    void VkRender::destroyUIPipeline(IUIRenderPipeline *pipeline)
    {
        destructor<VkUIPipeline>((VkUIPipeline *)pipeline);
        getSharedAlloctor().dealloctor(pipeline);
    }

    void VkRender::setupWindowFormat(VkWindow &window)
    {
        const VkFormat imageFormat[] = {VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
        const VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        uint32 fmtcnt = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(mDriver.mPhysical, window.mSurface, &fmtcnt, NULL);
        Vector<VkSurfaceFormatKHR> fmt;
        fmt.resize(fmtcnt);
        vkGetPhysicalDeviceSurfaceFormatsKHR(mDriver.mPhysical, window.mSurface, &fmtcnt, fmt.data());

        // 选择模式
        for (int i = 0; i < array_size(imageFormat, VkFormat); ++i)
            for (int j = 0; j < fmtcnt; ++j)
            {
                if (fmt[j].format == imageFormat[i] && fmt[j].colorSpace == colorSpace)
                {
                    window.mSurfaceFormat = fmt[j];
                    return;
                }
            }

        window.mSurfaceFormat = fmt[0];
    }

    void VkRender::setupWindowPresent(VkWindow &window)
    {
        VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
        uint32 fmtcnt = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(mDriver.mPhysical, window.mSurface, &fmtcnt, NULL);
        Vector<VkPresentModeKHR> fmt;
        fmt.resize(fmtcnt);
        vkGetPhysicalDeviceSurfacePresentModesKHR(mDriver.mPhysical, window.mSurface, &fmtcnt, fmt.data());

        for (int i = 0; i < array_size(present_modes, VkFormat); ++i)
            for (int j = 0; j < fmtcnt; ++j)
            {
                if (fmt[j] == present_modes[i])
                {
                    window.mPresentMode = fmt[j];
                    return;
                }
            }

        window.mPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    }
    void VkRender::setupWindowSwapchain(VkWindow &window, Bool resize)
    {
        VkResult err = vkDeviceWaitIdle(mDriver.mDevice);
        VkRenderDriver::check_vk_result(err);
        // 如果是改变窗口大小，先释放资源
        if (resize == Bool::True)
        {
        }
        // 重新创建
        {
            VkSwapchainKHR oldSwapchain = window.mSwapchain;
            VkSwapchainCreateInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            info.surface = window.mSurface;
            info.minImageCount = 2;
            info.imageFormat = window.mSurfaceFormat.format;
            info.imageColorSpace = window.mSurfaceFormat.colorSpace;
            info.imageArrayLayers = 1;
            info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Assume that graphics family == present family
            info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            info.presentMode = window.mPresentMode;
            info.clipped = VK_TRUE;
            info.oldSwapchain = oldSwapchain;
            VkSurfaceCapabilitiesKHR cap;
            err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDriver.mPhysical, window.mSurface, &cap);
            VkRenderDriver::check_vk_result(err);
            if (info.minImageCount < cap.minImageCount)
                info.minImageCount = cap.minImageCount;
            else if (cap.maxImageCount != 0 && info.minImageCount > cap.maxImageCount)
                info.minImageCount = cap.maxImageCount;

            if (cap.currentExtent.width == 0xffffffff)
            {
                info.imageExtent.width = window.mWidth;
                info.imageExtent.height = window.mHeight;
            }
            else
            {
                info.imageExtent.width = window.mWidth = cap.currentExtent.width;
                info.imageExtent.height = window.mHeight = cap.currentExtent.height;
            }
            err = vkCreateSwapchainKHR(mDriver.mDevice, &info, mDriver.mACallback, &window.mSwapchain);
            VkRenderDriver::check_vk_result(err);
            uint32 imgcnt = 0;
            err = vkGetSwapchainImagesKHR(mDriver.mDevice, window.mSwapchain, &imgcnt, NULL);
            VkRenderDriver::check_vk_result(err);
            VkImage backbuffers[16] = {};
            make_ensure(imgcnt >= 2);
            // make_ensure(wd->ImageCount < IM_ARRAYSIZE(backbuffers));
            err = vkGetSwapchainImagesKHR(mDriver.mDevice, window.mSwapchain, &imgcnt, backbuffers);
            VkRenderDriver::check_vk_result(err);

            // 创建帧同步信号量

            if (oldSwapchain != nullptr)
                vkDestroySwapchainKHR(mDriver.mDevice, oldSwapchain, mDriver.mACallback);
        }
    }

    void VkRender::setupWindow(VkWindow &window)
    {
        // 验证窗口系统是否支持
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(mDriver.mPhysical, mDriver.mGraphicsIndex, window.mSurface, &res);
        make_ensure(res == VK_TRUE);

        window.mSwapchain = nullptr;
        // 选择窗口表面格式
        setupWindowFormat(window);
        // 选择窗口呈现模式
        setupWindowPresent(window);
        // 创建窗口交换链等信息
        setupWindowSwapchain(window, Bool::False);
    }
}
