#ifndef __AIREVENTENUM__H__
#define __AIREVENTENUM__H__

#include <airkit/Core/airTypes.h>

namespace air
{
    // 事件类别
    enum class EventKind : uint16
    {
        Unknown, // 未知事件

        Window,    // 窗口事件
        Key,       // 键盘事件
        Mouse,     // 鼠标事件
        CharInput, // 文本输入事件

        Frame, // 帧渲染事件

        Max
    };
    // 事件动作
    enum class EventAct : uint16
    {
        Unknown, // 未知事件

        //---------------窗口事件------------------
        _WindowMin_,
        WindowRefresh, // 窗口刷新事件
        WindowClose,   // 窗口关闭事件
        WindowResize,  // 窗口大小事件
        WindowFocus,   // 窗口获得焦点
        WindowUnFocus, // 窗口失去焦点
        WindowMax,     // 窗口最大化事件
        WindowMin,     // 窗口最小化事件
        WindowRestore, // 窗口大小恢复事件
        WindowFrame,   // 窗口帧大小事件
        WindowScale,   // 窗口缩放事件
        WindowPos,     // 窗口位置事件
        WindowEnter,   // 鼠标进入窗口
        WindowLeave,   // 鼠标移出窗口
        _WindowMax_,
        //-------------------键盘事件----------------------
        _KeyMin_,
        KeyPress,   // 键盘按键按下
        KeyRepeat,  // 键盘按键一直按着
        KeyRelease, // 键盘按键释放
        _KeyMax_,
        //-------------------鼠标事件----------------------
        _MouseMin_,
        MousePress,    // 鼠标按键按下
        MouseRelease,  // 鼠标按键释放
        MousePos,      // 鼠标位置移动
        MouseScroll,   // 鼠标滚动
        MouseDblClick, // 鼠标双击事件
        _MouseMax_,

        Max
    };

    // 鼠标按键枚举
    enum class MouseButton : uint16
    {
        Unknown = 0,

        Btn0 = 1,
        Btn1,
        Btn2,
        Btn3,
        Btn4,
        Btn5,
        Btn6,
        Btn7,
        Max,

        BtnLast = Btn7, // 最后一个按键编码

        BtnLeft = Btn0,   // 左键
        BtnRight = Btn1,  // 右键
        BtnMiddle = Btn2, // 中键
    };
    // 键盘按键枚举
    enum class KeyButton : uint16
    {
        // From glfw3.h
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44,      /* , */
        Minus = 45,      /* - */
        Period = 46,     /* . */
        Slash = 47,      /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal = 61,     /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,  /* [ */
        Backslash = 92,    /* \ */
        RightBracket = 93, /* ] */
        GraveAccent = 96,  /* ` */

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* 功能性按键 */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* 小键盘区域 */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        // 控制性按键
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };
}

#endif //!__AIREVENTENUM__H__