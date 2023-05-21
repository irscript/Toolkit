#ifndef __AIRTHREAD__H__
#define __AIRTHREAD__H__

#include <airkit/Core/airTypes.h>

namespace air
{
    // 自旋锁
    class Spinlock
    {
    public:
        Spinlock(EBool shared = EBool::False);
        ~Spinlock();

        // 锁定
        void lock();
        void unlock();

    protected:
        uintptr mHandle; // 互斥量句柄
    };

    // 互斥量
    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

        // 锁定
        void lock();
        EBool trylock();
        void unlock();

    protected:
        uintptr mHandle; // 互斥量句柄
    };

    // 条件变量
    class Condition
    {
    public:
        Condition();
        ~Condition();

        void emit();
        void wait();

    protected:
        uintptr mMutex;
        uintptr mSemp;
    };
    // 线程局部存储变量
    class TLSV final
    {
    public:
        using Dealloctor = void (*)(uintptr);
        TLSV() : mHandle(0) {}
        TLSV(Dealloctor func) : mHandle(0) { create(func); }
        ~TLSV() { destroy(); }

        void create(Dealloctor func);
        void destroy();
        uintptr get();
        void set(uintptr ins);

    protected:
        uintptr mHandle;
    };

    using ThreadID = uintptr;
    // 线程定义
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();

        // 需要运行的主函数
        virtual uint worker() = 0;
        // 停止运行
        EBool start(EBool detach = EBool::True);
        // 停止运行
        void stop();
        // 线程分离
        EBool detach();

        // 线程是否运行
        EBool isRuing() { return mHanlde != 0 ? EBool::True : EBool::False; }
        // 线程能否分离
        EBool canDetach() { return mDetached != EBool::False ? EBool::True : EBool::False; }

        //------------全局静态函数------------
        static ThreadID getCurrentThreadID();
        static ThreadID initMainThreadID();
        static ThreadID getMainThreadID() { return mMainID; }
        static EBool isMainThread();
        static void *threadFunctionStatic(void *data);

    protected:
        volatile EBool mShouldRun : 1; // 是否继续运行
        volatile EBool mDetached : 1;  // 线程是否分离
        volatile EBool mStoped : 1;    // 线程是否已经结束运行
        ThreadID mHanlde;              // 线程句柄
        static ThreadID mMainID;       // 主线程ID
    };

}

#endif //!__AIRTHREAD__H__