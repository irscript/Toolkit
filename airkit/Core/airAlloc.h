#ifndef __AIRALLOC__H__
#define __AIRALLOC__H__

#include <airkit/Core/airIList.h>
// 内存分配器
namespace air
{
    // c++构造函数调用
    template <typename Type, typename... Args>
    inline Type *constructor(uintptr obj, Args... args)
    {
        Type *ret = (Type *)obj;
        ::new (obj) Type(args...);
        return ret;
    }
    // c++析构函数手动调用
    template <typename Type>
    inline void destructor(Type *obj) { obj->~Type(); }

    //_check_memory_free:检查内存释放情况
    // 初始化内存系统
    void initMemSys();
    // 释放内存系统
    void uninitMemSys();
// 内存分配
#ifdef _check_memory_free
    uintptr alloc_(uint size, cstring filepos, uint32 linepos);
#define alloc(size) alloc_(size, this_file(), this_line)
#else
    uintptr alloc(uint size);
#endif
    void dealloc(uintptr block);

    // 内存池
    template <typename T>
    struct MemPool
    {
        T *get();
        void free(T *block);
    };

}

#endif //!__AIRALLOC__H__