#ifndef __AIRVKRENDER__H__
#define __AIRVKRENDER__H__

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <airkit/Render/airRender.h>
#include <airkit/VKRender/airVkRenderDriver.h>
#include <airkit/VKRender/airVkRender.h>

namespace air
{
    struct VkWindow;
    // vulkan 渲染器
    struct VkRender : public IRender
    {
        // 初始化
        virtual bool init() override;
        // 释放资源
        virtual void terminal() override;
        // 创建窗口
        virtual IWindow *createWindow(const String &title, flt32 withd, flt32 height) override;
        // 释放窗口
        virtual void destroyWindow(IWindow *window) override;
        // 创建UI渲染管线
        virtual IUIRenderPipeline *createUIPipeline() override;
        // 释放UI渲染管线
        virtual void destroyUIPipeline(IUIRenderPipeline *pipeline) override;

    protected:
        // 选择窗口表面格式
        void setupWindowFormat(VkWindow &window);
        // 选择窗口呈现模式
        void setupWindowPresent(VkWindow &window);
        // 初始化窗口
        void setupWindow(VkWindow &window);
        // 创建窗口交换链等设备
        void setupWindowSwapchain(VkWindow &window, Bool resize = Bool::False);

    protected:
        VkRenderDriver mDriver;
    };
}
#endif //!__AIRVKRENDER__H__