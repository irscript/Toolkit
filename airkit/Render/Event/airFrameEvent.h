#ifndef __AIRFRAMEEVENT__H__
#define __AIRFRAMEEVENT__H__

#include <airkit/Render/Event/airIEvent.h>
namespace air
{
    // 帧渲染事件
    struct FrameEvent : public IEvent
    {
        FrameEvent()
            : IEvent(EventKind::Frame, EventAct::Unknown), mFrameTime(0), mEventTime(0), mSubTime(0) {}

        // 计算差值
        inline real calSub() { return mSubTime = mEventTime - mFrameTime; }

        // 单位：秒
        // 上一帧渲染时间（单位：秒）
        real mFrameTime;
        // 本次渲染事件时间
        real mEventTime;
        // 差值 = mEventTime - mFrameTime
        real mSubTime;
    };
}
#endif //!__AIRFRAMEEVENT__H__