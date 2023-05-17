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

    // 内存分配系统
    // 内存块头
    struct BlockHead : public IListNode<BlockHead>
    {
        // 内存debug信息
#ifdef _check_memory_free
        cstring mDbgFile;
        uint32 mDbgLine;
#endif
        uintptr mData[]; // 空数组
    };
    // 内存池头
    struct PoolkHead : public IListNode<PoolkHead>
    {
        // 初始化池
        void initPool(uint32 size);
        uint32 mBlkSize;          // 池内单个分块大小
        uint32 mBlkCount;         // 池内分块个数
        IList<BlockHead> mBlocks; // 未使用块链表
    };
    // 巨页内存头
    struct AreaHead : public IListNode<AreaHead>
    {
        // 初始化页
        void initArea();
        uint32 mPoolCount;       // 页内分池个数
        IList<PoolkHead> mPools; // 未使用池链表
    };
    // 内存池分配中心
    struct MemPoolCenter
    {
        MemPoolCenter() { init(); }
        // 初始化
        inline void init() { mArealist.initList(); }
        // 终止化
        void terminal();
        // 获取一个页
        AreaHead *getArea();
        // 获取一个池
        PoolkHead *getPool();
        // 释放一个池
        void freePool(PoolkHead *pool);

    private:
        IList<AreaHead> mArealist; // 巨页链表
    };
    // 通用内存分配器
    struct MemAlloctor
    {
        MemAlloctor();
        ~MemAlloctor();

        MemPoolCenter mCenter;           // 内存池分配中心
        IList<PoolkHead> mPoollist[32];  // 使用的池链表
        IList<BlockHead> mBlocklist[32]; // 使用的块链表
        IList<BlockHead> mBigslist;      // 大内存块

        uintptr alloctor(
            uint size
#ifdef _check_memory_free
            ,
            cstring filepos, uint32 linepos
#endif
        );
        void dealloctor(uintptr blk);
    };

    //_check_memory_free:检查内存释放情况
    // 初始化内存系统
    void initMemSys();
    // 释放内存系统
    void uninitMemSys();
// 内存分配
#ifdef _check_memory_free
    uintptr alloc_(uint size, cstring filepos, uint32 linepos);
#define alloc(size) alloc_(size, this_file(), this_line())
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