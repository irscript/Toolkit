#ifndef __AIRALLOC__H__
#define __AIRALLOC__H__

#include <airkit/Plat/airThread.h>

// 内存分配器
namespace air
{

    void assertion(cstring szExp,cstring szFile,cstring szFunc,int line);
    //_check_memory_free:检查内存释放情况
    // 初始化内存系统
    void initMemSys();
    // 释放内存系统
    void terminalMemSys();

    // 打印内存bug信息
    void checkMemSys();

// 内存分配
#ifdef _check_memory_free
    uintptr alloc_(uint size, cstring filepos, uint32 linepos);
#define alloc(size) alloc_(size, this_file(), this_line())
#else
    uintptr alloc(uint size);
#endif
    void dealloc(uintptr block);

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
                obj = (Type *)alloc(sizeof(Type));
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
                dealloc(obj);
            }
        }
    };

    // 内存分配器
    struct Alloctor
    {
        // 获取内存
        template <typename Type, typename... Args>
        inline static Type *getOne(Args... args)
        {
            auto obj = (Type *)alloc(sizeof(Type));
            constructor<Type>(obj, args...);
            return obj;
        }
        // 释放内存
        template <typename Type>
        inline static void freeOne(Type *block)
        {
            destructor<Type>(block);
            dealloc(block);
        }
    };

}

#endif //!__AIRALLOC__H__