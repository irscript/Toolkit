#ifndef __AIRVKWINDOW__H__
#define __AIRVKWINDOW__H__

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <airkit/Render/airRender.h>
#include <airkit/VKRender/airVkRenderDriver.h>
namespace air
{
    // 帧缓存信息描述
    struct VKFrameInfo
    {
        VkCommandPool CommandPool;
        VkCommandBuffer CommandBuffer;
        VkFence Fence;
        VkImage Backbuffer;
        VkImageView BackbufferView;
        VkFramebuffer Framebuffer;
        VKFrameInfo() { memset(this, 0, sizeof(VKFrameInfo)); }
    };
    // 帧缓存同步信号
    struct VKFrameSemaphores
    {
        VkSemaphore ImageAcquiredSemaphore;
        VkSemaphore RenderCompleteSemaphore;
        VKFrameSemaphores() { memset(this, 0, sizeof(VKFrameSemaphores)); }
    };
    struct VkWindow : public GlfwWindow
    {
        VkWindow();
        ~VkWindow();

    protected:
        // 初始化窗口
        bool setupWindow(VkRenderDriver &driver, const String &title, flt32 width, flt32 height);
        // 选择表面格式
        void selectSurfaceFormat(VkRenderDriver &driver);
        // 选择呈现模式
        void selectPresentMode(VkRenderDriver &driver);
        // 创建窗口交换链
        void createSwapchain(VkRenderDriver &driver);
        // 创建窗口命令池

    protected:
        friend struct VkRender;
        friend struct VkUIPipeline;
        

        VkSwapchainKHR mSwapchain; // 交换链
        VkSurfaceKHR mSurface;     // 窗口表面

        VkSurfaceFormatKHR mSurfaceFmt; // 表面格式
        VkPresentModeKHR mPresentMode;     // 呈现模式
        VkRenderPass mRenderPass;          // 渲染通道


    };
}
#endif //!__AIRVKWINDOW__H__