#ifndef __AIRUIFLAG__H__
#define __AIRUIFLAG__H__

#include <airkit/Core/airTypes.h>
// GUI 控件基类
namespace air
{
    // UI控件内部标记枚举
    enum class UIFlag : uint32
    {
        //---------通用标识[0,15]---------
        Window = bits32(0),   // 用于识别UI是不是窗口
        Focus = bits32(1),    // UI是否获得焦点
        Visible = bits32(2),  // UI是否可见
        Enable = bits32(3),   // UI是否可使用
        Movable = bits32(4),  // UI是否可移动
        Dragable = bits32(5), // UI是否可拖曳
        Drawable = bits32(6), // UI是否可绘制，在父窗口中是否可见
        Sizeable = bits32(7), // UI大小是否可变

        //----------控件专属标识[16,31]--------
    };

    constexpr inline uint32 operator|(const UIFlag a, const UIFlag b) { return (uint32)a | (uint32)b; }
    constexpr inline uint32 operator|(const uint32 flg, const UIFlag a) { return flg | (uint32)a; }
    constexpr inline uint32 operator|(const UIFlag a, const uint32 flg) { return flg | (uint32)a; }

    constexpr inline uint32 operator&(const uint32 flg, const UIFlag a) { return flg & (uint32)a; }
    constexpr inline uint32 operator&(const UIFlag a, const uint32 flg) { return flg & (uint32)a; }

    constexpr inline uint32 &operator|=(uint32 &flg, const UIFlag a) { return flg |= (uint32)a; }

    constexpr inline uint32 operator~(const UIFlag a) { return ~((uint32)a); }
}
#endif //!__AIRUIFLAG__H__