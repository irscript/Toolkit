#include <airkit/VKRender/airVkRender.h>
#include <airkit/Plat/airLoger.h>
#include <airkit/VKRender/airVkWindow.h>
#include <airkit/VKRender/airVkUIRenderPipeline.h>

namespace air
{

    void glfwErrorCallBack(int error, const char *description)
    {
        errlog("Glfw Error %d: %s\n", error, description);
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
        // 先分配内存
        auto window = (VkWindow *)salloc(sizeof(VkWindow));
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        if (window->setupWindow(mDriver, title, width, height) == false)
        {
            sdealloc(window);
            window = nullptr;
        }
        return window;
    }
    void VkRender::destroyWindow(IWindow *window)
    {
        auto &win = *(VkWindow *)window;
    }
   
}
