#ifndef __AIRKEYEVENT__H__
#define __AIRKEYEVENT__H__
#include <airkit/Render/Event/airIEvent.h>

namespace air
{
    // 键盘按键事件
    struct KeyEvent : public IEvent
    {
        KeyEvent(EventAct act, KeyButton key)
            : IEvent(EventKind::Key, act), mKey(key) {}

        KeyButton mKey;
    };

    // 字符输入事件
    struct CharInputEvent : public IEvent
    {
        CharInputEvent(uint32 code)
            : IEvent(EventKind::CharInput, EventAct::Unknown),
              mCode(code) {}

        uint32 mCode;
    };
}
#endif //!__AIRKEYEVENT__H__