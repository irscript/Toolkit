#ifndef __AIRVKWINDOW__H__
#define __AIRVKWINDOW__H__

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <airkit/Render/airRender.h>

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
    struct VkWindow : public IWindow
    {
        VkWindow() : IWindow() {}

    protected:
        inline GLFWwindow *glfwWindow() { return (GLFWwindow *)mNative; }

    protected:
        friend struct VkRender;
        friend struct VkUIPipeline;

        uint32 mWidth;             // 窗口宽度
        uint32 mHeight;            // 窗口高度
        String mTitle;             // 窗口标题
        
        VkSwapchainKHR mSwapchain; // 交换链
        VkSurfaceKHR mSurface;     // 窗口表面

        VkSurfaceFormatKHR mSurfaceFormat; // 表面格式
        VkPresentModeKHR mPresentMode;     // 呈现模式
        VkRenderPass mRenderPass;          // 渲染通道

        Vector<VKFrameInfo> mFrame;           // 帧缓存
        Vector<VKFrameSemaphores> mSemaphore; // 帧同步信号
    };
}
#endif //!__AIRVKWINDOW__H__