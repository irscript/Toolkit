#include <airkit/Plat/airLoger.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

namespace air
{
    SLoger print(nullptr, SLoger::White);

    SLoger memlog("MemSys", SLoger::LightRed);
#ifdef _check_trace_log
    SLoger tracelog("Trace", SLoger::Aqua);
#endif
    Spinlock SLoger::mLock;

    void SLoger::print(cstring msg)
    {
        mLock.lock();
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
        if (mPrev != nullptr)
            printf("[ %s ]: %s\n", mPrev, msg);
        else
            printf(msg);
        mLock.unlock();
    }
    // 打印日志
    void SLoger::printfmt(cstring fmt, ...)
    {
        mLock.lock();
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
        if (mPrev != nullptr)
            printf("[ %s ]: ", mPrev);
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(mBuffer, sizeof(mBuffer), fmt, ap);
        va_end(ap);
        printf(mBuffer);
        printf("\n");
        mLock.unlock();
    }
    void SLoger::print2(cstring msg)
    {
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
        if (mPrev != nullptr)
            printf("[ %s ]: %s\n", mPrev, msg);
        else
            printf(msg);
    }
    // 追加日志
    void SLoger::append(cstring fmt, ...)
    {
        HANDLE hCns = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCns, mClr);
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