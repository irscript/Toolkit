#include <airkit/Plat/airThread.h>
#include <airkit/Core/airAlloc.h>
#include <pthread.h>
#include <semaphore.h>

namespace air
{
    Spinlock::Spinlock(Bool shared) : mHandle(0)
    {
        pthread_spinlock_t *spin = (pthread_mutex_t *)&mHandle;
        pthread_spin_init(spin, shared == Bool::True ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE);
    }

    Spinlock::~Spinlock()
    {
        pthread_spin_destroy((pthread_mutex_t *)&mHandle);
        mHandle = 0;
    }

    void Spinlock::lock()
    {
        pthread_spin_lock((pthread_mutex_t *)&mHandle);
    }

    void Spinlock::unlock()
    {
        pthread_spin_unlock((pthread_mutex_t *)&mHandle);
    }

    Mutex::Mutex() : mHandle(0)
    {
        // 初始化互斥量
        pthread_mutex_t *mutex = (pthread_mutex_t *)&mHandle;
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(mutex, &attr);
    }

    Mutex::~Mutex()
    {
        // 释放互斥量
        pthread_mutex_t *mutex = (pthread_mutex_t *)&mHandle;
        pthread_mutex_destroy(mutex);
        mHandle = 0;
    }

    void Mutex::lock()
    {
        pthread_mutex_lock((pthread_mutex_t *)&mHandle);
    }

    Bool Mutex::trylock()
    {
        return pthread_mutex_trylock((pthread_mutex_t *)&mHandle) == 0
                   ? Bool::True
                   : Bool::False;
    }

    void Mutex::unlock()
    {
        pthread_mutex_unlock((pthread_mutex_t *)&mHandle);
    }

    Condition::Condition()
        : mMutex(0), mSemp(0)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init((pthread_mutex_t *)&mMutex, &attr);

        pthread_cond_init((pthread_cond_t *)mSemp, 0);
    }
    Condition::~Condition()
    {
        pthread_mutex_destroy((pthread_mutex_t *)&mMutex);
        pthread_cond_destroy((pthread_cond_t *)mSemp);
        mMutex = 0;
        mSemp = 0;
    }

    void Condition::emit()
    {
        pthread_cond_signal((pthread_cond_t *)mSemp);
    }
    void Condition::wait()
    {
        pthread_cond_wait((pthread_cond_t *)mSemp, (pthread_mutex_t *)&mMutex);
    }

    void TLSV::create(Dealloctor func)
    {
        if (mHandle == 0)
        {
            pthread_key_t *key = (pthread_key_t *)&mHandle;
            pthread_key_create(key, func);
        }
    }
    void TLSV::destroy()
    {
        if (mHandle != 0)
        {
            pthread_key_t *key = (pthread_key_t *)&mHandle;
            pthread_key_delete(*key);
            mHandle = 0;
        }
    }
    uintptr TLSV::get()
    {
        pthread_key_t *key = (pthread_key_t *)&mHandle;
        return pthread_getspecific(*key);
    }
    void TLSV::set(uintptr ins)
    {
        pthread_key_t *key = (pthread_key_t *)&mHandle;
        pthread_setspecific(*key, ins);
    }

    ThreadID Thread::mMainID = 0;
    Thread::Thread()
        : mHanlde(0),
          mShouldRun(Bool::False),
          mDetached(Bool::False),
          mStoped(Bool::False)
    {
    }

    Thread::~Thread()
    {
        stop();
    }

    void *Thread::threadFunctionStatic(void *data)
    {
        Thread *thread = static_cast<Thread *>(data);
        auto ret = thread->worker();
        checkMemSys();
        thread->mStoped = Bool::True;
        // pthread_exit((void *)0);
        return (void *)ret;
    }

    Bool Thread::start(Bool detach)
    {
        if (mHanlde != 0)
            return Bool::False;

        mShouldRun = Bool::True;
        mStoped = Bool::False;
        mDetached = detach;

        pthread_attr_t type;
        pthread_attr_init(&type);
        pthread_attr_setdetachstate(&type,
                                    detach == Bool::True ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
        pthread_create((pthread_t *)&mHanlde, &type, threadFunctionStatic, this);

        auto ret = mHanlde != 0 ? Bool::True : Bool::False;
        // 没有创建成功
        if (ret == Bool::False)
        {
            mShouldRun = Bool::False;
            mDetached = Bool::False;
        }
        return ret;
    }

    void Thread::stop()
    {
        if (mHanlde == 0)
            return;

        mShouldRun = Bool::False;
        if (mDetached == Bool::False)
            pthread_join((pthread_t)mHanlde, 0);

        while (mStoped != Bool::True)
            ;

        mHanlde = 0;
    }

    Bool Thread::detach()
    {
        // 已经分离
        if (mHanlde == 0 || mDetached == Bool::True)
            return Bool::False;
        // 分离失败
        if (pthread_detach((pthread_t)mHanlde) == 0)
        {
            mDetached = Bool::True;
            return Bool::True;
        }
        return Bool::False;
    }

    ThreadID Thread::getCurrentThreadID()
    {
        return (ThreadID)pthread_self();
    }

    ThreadID Thread::initMainThreadID()
    {
        mMainID = (ThreadID)pthread_self();
        pthread_tls_init();
        return mMainID;
    }

    Bool Thread::isMainThread()
    {
        auto id = (ThreadID)pthread_self();

        return mMainID == id ? Bool::True : Bool::False;
    }

}