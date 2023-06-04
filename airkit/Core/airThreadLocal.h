#ifndef __AIRTHREADLOCAL__H__
#define __AIRTHREADLOCAL__H__
#include <airkit/Core/airAlloc.h>
namespace air
{
    // 线程局部变量模板封装
    template <class Type>
    class ThreadLocal
    {
    public:
        ThreadLocal()
            : mInstance(ThreadLocal::destroy) {}

        Type &instance()
        {
            Type *obj = (Type *)mInstance.get();
            if (obj == nullptr)
            {
                obj = (Type *)talloc(sizeof(Type));
                constructor<Type>(obj);
                mInstance.set(obj);
            }
            return *obj;
        }

    private:
        TLSV mInstance;

        ThreadLocal(const ThreadLocal &) = delete;
        ThreadLocal &operator=(const ThreadLocal &) = delete;
        // 内存释放
        static void destroy(uintptr ins)
        {
            if (ins != nullptr)
            {
                Type *obj = (Type *)ins;
                destructor<Type>(obj);
                tdealloc(obj);
            }
        }
    };

}
#endif //!__AIRTHREADLOCAL__H__