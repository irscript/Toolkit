#ifndef __AIRIEVENT__H__
#define __AIRIEVENT__H__

#include <airkit/Render/Event/airEventEnum.h>

namespace air
{

    // 事件基类
    struct IEvent
    {
        IEvent(EventKind kind, EventAct type)
            : mKind(kind), mAct(type), mHandle(Bool::False) {}
        // 获取事件类别
        inline EventKind GetKind() { return mKind; }
        // 获取事件动作
        inline EventAct GetAct() { return mAct; }
        // 更新事件动作
        inline void SetAct(EventAct act) { mAct = act; }
        // 事件是否已处理
        inline int32 IsHandled() { return mHandle == Bool::True; }

    protected:
        Bool mHandle;    // 事件是否已处理
        EventKind mKind; // 事件类别
        EventAct mAct;   // 事件动作
    };

    // 事件监听器接口
    class IEventListener
    {
    public:
        virtual ~IEventListener() = default;
        // 过滤事件，返回值为true：表示未过滤该事件，false：丢弃事件
        virtual bool OnEvent(IEvent &envent) = 0;
    };
}

#endif //!__AIRIEVENT__H__