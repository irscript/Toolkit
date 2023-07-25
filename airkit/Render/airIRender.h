#ifndef __AIRIRENDER__H__
#define __AIRIRENDER__H__

#include <airkit/Render/airUIRenderPipeline.h>

namespace air
{

    // 渲染器接口
    struct IRender
    {
        // 初始化
        virtual bool init() = 0;
        // 释放资源
        virtual void terminal() = 0;
        // 创建窗口
        virtual IWindow *createWindow(const String &title, flt32 withd, flt32 height) = 0;
        // 释放窗口
        virtual void destroyWindow(IWindow *window) = 0;
        
    };
}
#endif //!__AIRIRENDER__H__