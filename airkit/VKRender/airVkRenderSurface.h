#ifndef __AIRVKRENDERSURFACE__H__
#define __AIRVKRENDERSURFACE__H__

#include <airkit/VKRender/airVkRenderDriver.h>

namespace air
{
    // Vulkan窗口渲染集成，与窗口不偶合
    struct VkRenderSurface
    {
        VkRenderSurface();

        VkSwapchainKHR mSwapchain; // 交换链
        VkSurfaceKHR mSurface;     // 窗口表面
        VkExtent2D mExtent;        // 窗口表面尺寸

        VkSurfaceFormatKHR mSurfaceFmt; // 表面格式
        VkPresentModeKHR mPresentMode;  // 呈现模式
        VkRenderPass mRenderPass;       // 渲染通道

        VkPipeline mPipeline;           // 渲染管线

        uint32 mMinImgCount; // 渲染模式的缓冲数量

        // 功能函数
        // 初始化窗口
        bool setupWindow(VkRenderDriver &driver, uint32 width, uint32 height);
        // 选择表面格式
        void selectSurfaceFormat(VkRenderDriver &driver);
        // 选择呈现模式
        void selectPresentMode(VkRenderDriver &driver);
        // 创建窗口交换链
        void createSwapchain(VkRenderDriver &driver, uint32 width, uint32 height);
        // 创建图像视图
        void createImgeView();
        // 创建渲染通道
        
        // 创建渲染管线

        // 通过呈现模式获取最小显示缓存图像数目
        void getMinImgCount();
    };
}

#endif