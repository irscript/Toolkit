#ifndef __AIRWINDOWEVENT__H__
#define __AIRWINDOWEVENT__H__

#include <airkit/Render/Event/airIEvent.h>

namespace air
{
    // 窗口事件
    struct WindowEvent : public IEvent
    {
        WindowEvent(EventAct act)
            : IEvent(EventKind::Window, act) {}
        WindowEvent(EventAct act, int32 arg1, int32 arg2)
            : IEvent(EventKind::Window, act),
              mWidth(arg1), mHeight(arg2) {}
        WindowEvent(EventAct act, flt32 arg1, flt32 arg2)
            : IEvent(EventKind::Window, act),
              mXScale(arg1), mYScale(arg2) {}

        // 数据成员
        union
        {
            // 窗口大小
            struct
            {
                int32 mWidth, mHeight;
            };
            // 窗口位置事件
            struct
            {
                int32 mXpos, mYpos;
            };
            // 窗口缩放
            struct
            {
                flt32 mXScale, mYScale;
            };
        };
    };
}

#endif //!__AIRWINDOWEVENT__H__