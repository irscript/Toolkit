#include "airVkRenderSurface.h"

#include <airkit/Plat/airLoger.h>
namespace air
{
    VkRenderSurface::VkRenderSurface()
    {
        mSwapchain = nullptr;
        mSurface = nullptr;
        mSurfaceFmt.colorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR;
        mSurfaceFmt.format = VkFormat::VK_FORMAT_MAX_ENUM;
        mPresentMode = VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR;
    }

    bool VkRenderSurface::setupWindow(VkRenderDriver &driver, uint32 width, uint32 height)
    {
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
    }
    void VkRenderSurface::selectSurfaceFormat(VkRenderDriver &driver)
    {
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
                mSurfaceFmt.format = requestSurfaceImageFormat[0];
                mSurfaceFmt.colorSpace = requestSurfaceColorSpace;
            }
            else
            {
                mSurfaceFmt = availfmt[0];
            }
        }
        else
        {
            for (int request_i = 0; request_i < array_size_ext(requestSurfaceImageFormat); request_i++)
                for (uint32_t avail_i = 0; avail_i < availcnt; avail_i++)
                    if (availfmt[avail_i].format == requestSurfaceImageFormat[request_i] && availfmt[avail_i].colorSpace == requestSurfaceColorSpace)
                    {
                        mSurfaceFmt = availfmt[avail_i];
                        return;
                    }
            mSurfaceFmt = availfmt[0];
        }
    }
    void VkRenderSurface::selectPresentMode(VkRenderDriver &driver)
    {
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
                    mPresentMode = present_modes[request_i];
                    return;
                }
        // 如果没有匹配
        mPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    }
    void VkRenderSurface::createSwapchain(VkRenderDriver &driver, uint32 width, uint32 height)
    {
        // 等待设备就绪
        VkResult err = vkDeviceWaitIdle(driver.mDevice);
        VkRenderDriver::checkVKResult(err);

        getMinImgCount();

        // 创建交换链
        {
            auto oldswapchain = mSwapchain;
            VkSwapchainCreateInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            info.surface = mSurface;
            info.minImageCount = mMinImgCount;
            info.imageFormat = mSurfaceFmt.format;
            info.imageColorSpace = mSurfaceFmt.colorSpace;
            info.imageArrayLayers = 1;
            info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Assume that graphics family == present family
            info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            info.presentMode = mPresentMode;
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
                info.imageExtent.width = mExtent.width = width;
                info.imageExtent.height = mExtent.height = height;
            }
            else
            {
                info.imageExtent.width = mExtent.width = cap.currentExtent.width;
                info.imageExtent.height = mExtent.height = cap.currentExtent.height;
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
    void VkRenderSurface::getMinImgCount()
    {
        switch (mPresentMode)
        {
        case VK_PRESENT_MODE_MAILBOX_KHR:
        {
            mMinImgCount = 3;
        }
        break;
        case VK_PRESENT_MODE_FIFO_KHR:
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
        {
            mMinImgCount = 3;
        }
        break;
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
        {
            mMinImgCount = 3;
        }
        break;
        default:
        {
            mMinImgCount = 1;
        }
        break;
        }
    }
}
