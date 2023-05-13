#include <airkit/Core/airAlloc.h>
#include <airkit/Core/airIList.h>

#include <stdlib.h>
#include <stdio.h>

namespace air
{

// 池地址对齐大小
#define _area_pool_base (0x1000) // 4k
// 巨页内存大小
#define _mem_area_size (0x400000) // 4M 4 * 1024 * 1024
// 内存池大小
#define _mem_pool_size (0x1000) // 4K 4 * 1024

// 内存块最大值
#define _block_max_size (256)
// 内存块对齐大小
#define _block_alig_size (8)
// 内存池的种类
#define _pool_count (_block_max_size / _block_alig_size) // 32

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
        uint32 mBlkSize;          // 池内单个分块大小
        uint32 mBlkCount;         // 池内分块个数
        IList<BlockHead> mBlocks; // 未使用块链表
    };

    //-------------内存系统分配释放----------------------------
    // 初始化内存系统
    void initMemSys() {}
    // 释放内存系统
    void uninitMemSys() {}
#ifdef _check_memory_free
    uintptr alloc_(uint size, cstring filepos, uint32 linepos)
#else
    uintptr alloc(uint size)
#endif
    {
    }

    void dealloc(uintptr block)
    {
    }
}