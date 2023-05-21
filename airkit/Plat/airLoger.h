#ifndef __AIRLOGER__H__
#define __AIRLOGER__H__

#include <airkit/Core/airTypes.h>
#include <airkit/Plat/airThread.h>
namespace air
{

    struct SLoger
    {
        // 日志颜色
        enum
        {
            Black,       // 黑色
            Blue,        // 蓝色
            Green,       // 绿色
            Wathet,      // 浅蓝色
            Red,         // 红色
            Purple,      // 紫色
            Yellow,      // 黄色
            White,       // 白色
            Grey,        // 灰色
            LightBlue,   // 淡蓝色
            LightGreen,  // 淡绿色
            Aqua,        // 淡浅绿色
            LightRed,    // 淡红色
            Lavender,    // 淡紫色
            FaintYellow, // 淡黄色
            BrightWhite, // 亮白色
        };
        SLoger(cstring prefix, uint32 frt, uint32 bkg = SLoger::Black)
            : mPrev(prefix), mClr(frt | bkg) {}

        // 打印日志
        void print(cstring msg);
        void printfmt(cstring fmt, ...);

        inline void start() { mLock.lock(); }
        void print2(cstring msg);
        // 追加日志
        void append(cstring fmt, ...);
        // 带颜色追加日志
        void appendWithClr(uint32 clr, cstring fmt, ...);
        inline void end() { mLock.unlock(); }

        template <typename... Arg>
        inline constexpr void operator()(cstring fmt, Arg... arg)
        {
            if (sizeof...(Arg) == 0)
                print(fmt);
            else
                printfmt(fmt, arg...);
        }

    private:
        cstring mPrev;      // 日志前缀
        uint32 mClr;        // 日志颜色
        char mBuffer[2048]; // 日志缓存

        static Spinlock mLock;
    };

    // 内存日志系统
    extern SLoger memlog;

    // 开启跟踪日志系统
#ifdef _check_trace_log
    // 跟踪日志系统
    extern SLoger tracelog;
#endif
}

#endif //!__AIRLOGER__H__