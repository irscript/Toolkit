#ifndef __AIRTHREAD__H__
#define __AIRTHREAD__H__

#include <airkit/Core/airTypes.h>

namespace air
{
    // 自旋锁
    class Spinlock
    {
    public:
        Spinlock(Bool shared = Bool::False);
        ~Spinlock();

        // 锁定
        void lock();
        void unlock();

    protected:
        uintptr mHandle; // 互斥量句柄
    };
    class Spinlocker
    {
    public:
        inline Spinlocker() : mHandle() { mHandle.lock(); }
        inline ~Spinlocker() { mHandle.unlock(); }

    protected:
        Spinlock mHandle; // 互斥量句柄
    };

    // 互斥量
    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

        // 锁定
        void lock();
        Bool trylock();
        void unlock();

    protected:
        uintptr mHandle; // 互斥量句柄
    };
    class MutexLock
    {
    public:
        inline MutexLock() { mMutex.lock(); }
        inline ~MutexLock() { mMutex.unlock(); }

    protected:
        Mutex mMutex; // 互斥量句柄
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
        Bool start(Bool detach = Bool::True);
        // 停止运行
        void stop();
        // 线程分离
        Bool detach();

        // 线程是否运行
        Bool isRuing() { return mHanlde != 0 ? Bool::True : Bool::False; }
        // 线程能否分离
        Bool canDetach() { return mDetached != Bool::False ? Bool::True : Bool::False; }

        //------------全局静态函数------------
        static ThreadID getCurrentThreadID();
        static ThreadID initMainThreadID();
        static ThreadID getMainThreadID() { return mMainID; }
        static Bool isMainThread();
        static void *threadFunctionStatic(void *data);

    protected:
        volatile Bool mShouldRun : 1; // 是否继续运行
        volatile Bool mDetached : 1;  // 线程是否分离
        volatile Bool mStoped : 1;    // 线程是否已经结束运行
        ThreadID mHanlde;              // 线程句柄
        static ThreadID mMainID;       // 主线程ID
    };

}

#endif //!__AIRTHREAD__H__