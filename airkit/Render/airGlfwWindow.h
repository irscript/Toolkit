#ifndef __AIRGLFWWINDOW__H__
#define __AIRGLFWWINDOW__H__
#include <GLFW/glfw3.h>
#include <airkit/Render/UI/airGUIElement.h>

namespace air
{
    struct GlfwWindow : public IWindow
    {
        GlfwWindow() : IWindow() {}

    protected:
        inline GLFWwindow *glfwWindow() { return (GLFWwindow *)mNative; }

    protected:
        uint32 mWidth;  // 窗口宽度
        uint32 mHeight; // 窗口高度
        String mTitle;  // 窗口标题
    };
}
#endif //!__AIRGLFWWINDOW__H__
