#include "airVkWindow.h"
#include <airkit/Plat/airLoger.h>
#include <airkit/VKRender/airVkWindow.h>

namespace air
{
    VkWindow::VkWindow() : GlfwWindow()
    {
        mSwapchain = nullptr;
        mSurface = nullptr;
        mSurfaceFmt.colorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR;
        mSurfaceFmt.format = VkFormat::VK_FORMAT_MAX_ENUM;
        mPresentMode = VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR;
    }
    VkWindow::~VkWindow()
    {
    }

    bool VkWindow::setupWindow(VkRenderDriver &driver, const String &title, flt32 width, flt32 height)
    {
        // 创建窗口
        mNative = glfwCreateWindow(width, height, title.cstr(), nullptr, nullptr);
        make_ensure(mNative != nullptr);
        // 创建窗口曲面
        VkResult err = glfwCreateWindowSurface(driver.mInstance, glfwWindow(), driver.mAllocCall, &mSurface);
        VkRenderDriver::checkVKResult(err);
        // 检查WSI支持
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(driver.mPhysical, driver.mGraphicsIndex, mSurface, &res);
        if (res != VK_TRUE)
        {
            errlog("Error no WSI support on physical device 0\n");
            exit(-1);
        }
        // 选择表面格式
        selectSurfaceFormat(driver);
        // 选择呈现模式
        selectPresentMode(driver);
        return true;
    }
    void VkWindow::selectSurfaceFormat(VkRenderDriver &driver)
    {
        // 已经选择过
        if (driver.mSurfaceFmt.format != VkFormat::VK_FORMAT_MAX_ENUM)
        {
            mSurfaceFmt = driver.mSurfaceFmt;
            return;
        }

        // 请求的表面格式
        const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

        // 获取表面支持的格式
        uint32 availcnt = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(driver.mPhysical, mSurface, &availcnt, NULL);
        Vector<VkSurfaceFormatKHR> availfmt;
        availfmt.resize(availcnt);
        vkGetPhysicalDeviceSurfaceFormatsKHR(driver.mPhysical, mSurface, &availcnt, availfmt.data());
        // 选择支持的格式
        if (availcnt == 1)
        {
            if (availfmt[0].format == VK_FORMAT_UNDEFINED)
            {
                driver.mSurfaceFmt.format = requestSurfaceImageFormat[0];
                driver.mSurfaceFmt.colorSpace = requestSurfaceColorSpace;
            }
            else
            {
                driver.mSurfaceFmt = availfmt[0];
            }
        }
        else
        {
            for (int request_i = 0; request_i < array_size_ext(requestSurfaceImageFormat); request_i++)
                for (uint32_t avail_i = 0; avail_i < availcnt; avail_i++)
                    if (availfmt[avail_i].format == requestSurfaceImageFormat[request_i] && availfmt[avail_i].colorSpace == requestSurfaceColorSpace)
                    {
                        driver.mSurfaceFmt = availfmt[avail_i];
                        goto End;
                    }
            driver.mSurfaceFmt = availfmt[0];
        }
    End:
        mSurfaceFmt = driver.mSurfaceFmt;
    }

    void VkWindow::selectPresentMode(VkRenderDriver &driver)
    {
        // 已经选择过
        if (driver.mPresentMode != VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR)
        {
            mPresentMode = driver.mPresentMode;
            return;
        }
        // 请求的模式
        VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};

        // 获取支持的显示模式
        uint32 availcnt = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(driver.mPhysical, mSurface, &availcnt, NULL);
        Vector<VkPresentModeKHR> availfmt;
        availfmt.resize(availcnt);
        vkGetPhysicalDeviceSurfacePresentModesKHR(driver.mPhysical, mSurface, &availcnt, availfmt.data());
        // 选择支持的格式

        for (int request_i = 0; request_i < array_size_ext(present_modes); request_i++)
            for (uint32_t avail_i = 0; avail_i < availcnt; avail_i++)
                if (present_modes[request_i] == availfmt[avail_i])
                {
                    driver.mPresentMode = present_modes[request_i];
                    goto End;
                }
        // 如果没有匹配
        driver.mPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    End:
        mPresentMode = driver.mPresentMode;
    }
    void VkWindow::createSwapchain(VkRenderDriver &driver)
    {
        // 等待设备就绪
        VkResult err = vkDeviceWaitIdle(driver.mDevice);
        VkRenderDriver::checkVKResult(err);

        if (driver.mMinImgCount == -1)
            driver.getMinImgCount();

        // 创建交换链
        {
            auto oldswapchain = mSwapchain;
            VkSwapchainCreateInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            info.surface = mSurface;
            info.minImageCount = driver.mMinImgCount;
            info.imageFormat = driver.mSurfaceFmt.format;
            info.imageColorSpace = driver.mSurfaceFmt.colorSpace;
            info.imageArrayLayers = 1;
            info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Assume that graphics family == present family
            info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            info.presentMode = driver.mPresentMode;
            info.clipped = VK_TRUE;
            info.oldSwapchain = oldswapchain;
            // 验证显示能力
            VkSurfaceCapabilitiesKHR cap;
            err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(driver.mPhysical, mSurface, &cap);
            VkRenderDriver::checkVKResult(err);
            if (info.minImageCount < cap.minImageCount)
                info.minImageCount = cap.minImageCount;
            else if (cap.maxImageCount != 0 && info.minImageCount > cap.maxImageCount)
                info.minImageCount = cap.maxImageCount;

            if (cap.currentExtent.width == 0xffffffff)
            {
                info.imageExtent.width = mWidth;
                info.imageExtent.height = mHeight;
            }
            else
            {
                info.imageExtent.width = mWidth = cap.currentExtent.width;
                info.imageExtent.height = mHeight = cap.currentExtent.height;
            }
            // 创建交换链
            mSwapchain = nullptr;
            err = vkCreateSwapchainKHR(driver.mDevice, &info, driver.mAllocCall, &mSwapchain);
            VkRenderDriver::checkVKResult(err);
            // 获取图像

            // 释放之前的交换链
            if (oldswapchain)
                vkDestroySwapchainKHR(driver.mDevice, oldswapchain, driver.mAllocCall);
        }
        // 图像视图

        // 创建帧缓存
    }
}
