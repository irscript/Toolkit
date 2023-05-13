#include <airkit/Plat/airLoger.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

namespace air
{

    SLoger memlog("MemSys", SLoger::LightRed);
#ifdef _check_trace_log
    SLoger tracelog("Trace", SLoger::Aqua);
#endif
    void SLoger::print(cstring msg)
    {
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
        printf("[ %s ]: %s", mPrev, msg);
    }
    // 打印日志
    void SLoger::printfmt(cstring fmt, ...)
    {
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
        printf("[ %s ]: ", mPrev);
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(mBuffer, sizeof(mBuffer), fmt, ap);
        va_end(ap);
        printf(mBuffer);
    }
    // 追加日志
    void SLoger::append(cstring fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);

        vsnprintf(mBuffer, sizeof(mBuffer), fmt, ap);
        va_end(ap);
        printf(mBuffer);
    }
    // 带颜色追加日志
    void SLoger::appendWithClr(uint32 clr, cstring fmt, ...)
    {
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, clr);
        va_list ap;
        va_start(ap, fmt);

        vsnprintf(mBuffer, sizeof(mBuffer), fmt, ap);
        va_end(ap);
        printf(mBuffer);
    }
}