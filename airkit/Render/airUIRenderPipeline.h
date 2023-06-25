#ifndef __AIRUIRENDERPIPELINE__H__
#define __AIRUIRENDERPIPELINE__H__
#include <airkit/Render/UI/airGUIElement.h>
namespace air
{
    // UI渲染管线
    struct IUIRenderPipeline
    {
        // 渲染窗口
        virtual void onRenderWindow(IWindow *window) = 0;
    };
}
#endif //!__AIRUIRENDERPIPELINE__H__