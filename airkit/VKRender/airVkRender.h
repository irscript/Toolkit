#ifndef __AIRVKRENDER__H__
#define __AIRVKRENDER__H__

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <airkit/Render/airRender.h>
#include <airkit/VKRender/airVkRenderDriver.h>

namespace air
{
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
        

    protected:
        

    protected:
        VkRenderDriver mDriver;
    };
}
#endif //!__AIRVKRENDER__H__