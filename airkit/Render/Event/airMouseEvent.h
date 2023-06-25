#ifndef __AIRMOUSEEVENT__H__
#define __AIRMOUSEEVENT__H__

#include <airkit/Render/Event/airIEvent.h>

namespace air
{
    // 鼠标事件
    struct MouseEvent : public IEvent
    {
        // 鼠标按键事件
        MouseEvent(EventAct act, MouseButton btn)
            : IEvent(EventKind::Mouse, act), mBtn(btn) {}
        // 鼠标位置事件
        MouseEvent(EventAct act, uint32 xpos, uint32 ypos)
            : IEvent(EventKind::Mouse, act), mXpos(xpos), mYpos(ypos) {}
        // 鼠标滚动事件
        MouseEvent(EventAct act, flt32 xoffset, flt32 yoffset)
            : IEvent(EventKind::Mouse, act),
              mXOffset(mXOffset), mYOffset(yoffset) {}

    protected:
        union
        {
            // 鼠标位置
            struct
            {
                uint32 mXpos, mYpos;
            };
            // 鼠标滚动
            struct
            {
                flt32 mXOffset, mYOffset;
            };
            // 按键
            MouseButton mBtn;
        };
    };

    // 鼠标按键按下事件
    struct MouseBtnPress : public MouseEvent
    {
        MouseBtnPress(MouseButton btn)
            : MouseEvent(EventAct::MousePress, btn) {}

        inline MouseButton getBtn() { return mBtn; }
    };
    // 鼠标按键释放事件
    struct MouseBtnRelease : public MouseEvent
    {
        MouseBtnRelease(MouseButton btn)
            : MouseEvent(EventAct::MousePress, btn) {}

        inline MouseButton getBtn() { return mBtn; }
    };
    // 鼠标按键双击事件
    struct MouseDblClick : public MouseEvent
    {
        MouseDblClick(MouseButton btn)
            : MouseEvent(EventAct::MouseDblClick, btn) {}

        inline MouseButton getBtn() { return mBtn; }
    };
    // 鼠标位置移动事件
    struct MouseMove : public MouseEvent
    {
        MouseMove(uint32 xpos, uint32 ypos)
            : MouseEvent(EventAct::MousePos, xpos, ypos) {}

        inline uint32 xpos() const { return mXpos; }
        inline uint32 ypos() const { return mYpos; }
    };
    // 鼠标滚动事件
    struct MouseScroll : public MouseEvent
    {
        MouseScroll(flt32 xoffset, flt32 yoffset)
            : MouseEvent(EventAct::MouseScroll, xoffset, yoffset) {}

        inline flt32 xoffset() const { return mXOffset; }
        inline flt32 yoffset() const { return mYOffset; }
    };

}

#endif //!__AIRMOUSEEVENT__H__