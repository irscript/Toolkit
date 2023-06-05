#ifndef __AIRALLOC__H__
#define __AIRALLOC__H__

#include <airkit/Plat/airThread.h>

// 内存分配器
namespace air
{

    void assertion(cstring szExp, cstring szFile, cstring szFunc, int line);
    //_check_memory_free:检查内存释放情况
    // 初始化内存系统
    void initMemSys();
    // 释放内存系统
    void terminalMemSys();

    // 打印内存bug信息
    void checkMemSys();

    // 内存分配接口
    struct IAlloctor
    {

        virtual uintptr alloctor(uint size
#ifdef _check_memory_free
                                 ,
                                 cstring filepos, uint32 linepos
#endif
                                 ) = 0;

        virtual void dealloctor(uintptr block) = 0;
    };
    // 获取当前线程的内存分配器
    IAlloctor &getThreadAlloctor();
    // 获取多线程分配器
    IAlloctor &getMainAlloctor();
    // 线程局部分配器
    enum class AlloctorType
    {
        Thread, // 线程局部分配器
        Main,   // 主线程分配器
    };

    // 内存分配器
    template <typename Type, const AlloctorType mem = AlloctorType::Thread>
    struct Alloctor
    {
        Alloctor() : mInstance(nullptr)
        {
            if (mem == AlloctorType::Thread)
                mInstance = &getThreadAlloctor();
            else if (mem == AlloctorType::Main)
                mInstance = &getMainAlloctor();
        }
        // 获取内存
        template <typename... Args>
        inline Type *alloc(
#ifdef _check_memory_free
            cstring filepos, uint32 linepos,
#endif
            Args... args)
        {
            auto obj =
                (Type *)mInstance->alloctor(sizeof(Type)
#ifdef _check_memory_free
                                                ,
                                            filepos, linepos
#endif
                );
            constructor<Type>(obj, args...);
            return obj;
        }
        inline Type *allocArr(
            uint count
#ifdef _check_memory_free
            ,
            cstring filepos, uint32 linepos
#endif
        )
        {
            return (Type *)mInstance->alloctor(sizeof(Type) * count
#ifdef _check_memory_free
                                               ,
                                               filepos, linepos
#endif
            );
        }
        // 释放内存

        inline void dealloc(Type *block)
        {
            destructor<Type>(block);
            mInstance->dealloctor(block);
        }

    protected:
        IAlloctor *mInstance; // 分配器实例
    };

    //====================线程局部的内存分配管理器===========================

    // 线程内存分配
#ifdef _check_memory_free
#define talloc(size) alloc_(size, this_file(), this_line())
    inline uintptr alloc_(uint size, cstring filepos, uint32 linepos)
    {
        make_ensure(size != 0);
        return getThreadAlloctor().alloctor(size, filepos, linepos);
    }
#else
    inline uintptr alloc(uint size)
    {
        make_ensure(size != 0);
        return getThreadAlloctor().alloctor(size);
    }
#endif
    // 线程内存释放
    inline void tdealloc(uintptr block)
    {
        if (block != nullptr)
            getThreadAlloctor().dealloctor(block);
    }

}

#endif //!__AIRALLOC__H__