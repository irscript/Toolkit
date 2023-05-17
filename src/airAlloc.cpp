#include <airkit/Core/airAlloc.h>
#include <airkit/Core/airIList.h>
#include <airkit/Plat/airLoger.h>
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

    //-------------内存系统分配释放----------------------------
    // 初始化内存系统
    void initMemSys() {}
    // 释放内存系统
    void uninitMemSys() {}

    void PoolkHead::initPool(uint32 size)
    {
        // 初始化池
        mBlkSize = size;
        mBlkCount = 0;
        init();

        // 计算首个块的地址
        const uint address = (uint)this;
        uint blka = address + sizeof(PoolkHead);
        blka = alignup(blka, (uint)size);

        // 计算分块个数
        const uint sub = blka - address;
        uint32 blkcnt = (_mem_pool_size - sub) / (size + sizeof(BlockHead));
        // 初始化池内内存块
        for (uint32 i = 0; i < blkcnt; ++i)
        {
            auto blk = (BlockHead *)(blka + i * (size + sizeof(BlockHead)));
            constructor<BlockHead>(blk);
            // blk->init();
            mBlocks.insertTail(blk);
        }
        mBlkCount = blkcnt;
    }
    void AreaHead::initArea()
    {
        init();
        mPoolCount = 0;
        mPools.initList();
        // 对齐地址
        const uint address = (uint)this;
        uint poola = address + sizeof(AreaHead);
        poola = alignup(poola, (uint)_area_pool_base);
        const auto sub = poola - address;

        // 计算池的可用个数
        uint poolcnt = (_mem_area_size - sub) / _mem_pool_size;
        for (uint i = 0; i < poolcnt; ++i)
        {
            uint paddr = poola + i * _mem_pool_size;
            auto pool = (PoolkHead *)paddr;
            constructor<PoolkHead>(pool);
            mPools.insertTail(pool);
        }
        mPoolCount = (uint32)poolcnt;
    }

    void MemPoolCenter::terminal()
    {
        // 释放内存
        for (uint i = 0; i < mArealist.getCount(); ++i)
        {
            auto node = mArealist.removeEntry();
            free(node);
        }
    }

    AreaHead *MemPoolCenter::getArea()
    {
        auto area = (AreaHead *)malloc(_mem_area_size);
        if (area == nullptr)
            return nullptr;

        area->initArea();
        mArealist.insertEntry(area);
        return area;
    }
    PoolkHead *MemPoolCenter::getPool()
    {
        // 查找可分配的巨页
        AreaHead *area = nullptr;
        for (auto iter = mArealist.front();
             iter != mArealist.getRoot();
             iter = iter->getNext())
        {
            if (iter->mPools.getCount() > 0)
            {
                area = iter;
                break;
            }
        }
        // 没有找到页，分配一个
        if (area == nullptr)
        {
            area = getArea();
            if (area == nullptr)
            {
                memlog("获取巨页内存失败！\n");
                exit(1);
            }
        }
        // 查找池
        PoolkHead *pool = area->mPools.removeTail();

        return pool;
    }
    void MemPoolCenter::freePool(PoolkHead *pool)
    {
        if (pool == nullptr)
            return;
        uint pos = (uint)pool;
        // 找到在哪个巨页
        for (auto iter = mArealist.front();
             iter != mArealist.getRoot();
             iter = iter->getNext())
        {
            auto start = (uint)iter;
            auto end = (uint)(iter + _mem_area_size);
            if (start < pos && pos < end)
            {
                iter->mPools.insertEntry(pool);
                // 页是否使用完毕,使用完毕释放内存
                if (iter->mPoolCount == iter->mPools.getCount())
                {
                    mArealist.remove(iter);
                    free(iter);
                }
                return;
            }
        }
        memlog("未知池：0x%lx\n", pool);
    }

    MemAlloctor::MemAlloctor()
    {
        mCenter.init();
        for (uint32 i = 0; i < 32; ++i)
        {
            mPoollist[i].initList();
            mBlocklist[i].initList();
        }
        mBigslist.initList();
    }
    MemAlloctor::~MemAlloctor()
    {
        // 释放通用内存块
#ifdef _check_memory_free
        uint dbgcnt = 0;
        memlog("------------------checking-free-start---------------------\n");
        // 通用内存块
        for (uint i = 0; i < 32; ++i)
        {
            auto size = (i + 1) * 8;
            for (auto iter = mBlocklist[i].getEntry();
                 iter != mBlocklist[i].getRoot();
                 iter = iter->getNext())
            {
                memlog("checking-free>addr: 0x%lx\tsize: %d \tin [file: %s\tline: %d]!\n",
                       iter, size,
                       iter->mDbgFile, iter->mDbgLine);
                ++dbgcnt;
            }
        }
        // 大内存
        for (auto iter = mBigslist.getEntry();
             iter != mBigslist.getRoot();
             iter = iter->getNext())
        {
            memlog("checking-big-free>addr: 0x%lx\tin [file: %s\tline: %d ]!\n",
                   iter,
                   iter->mDbgFile, iter->mDbgLine);
            ++dbgcnt;
        }
        memlog("------------------checking-free-end----------------------\n");
        memlog("checking-free-count: %ld\n\n", dbgcnt);
#endif
        mCenter.terminal();

        // 释放大内存块
        auto cnt = mBigslist.getCount();
        for (uint i = 0; i < cnt; ++i)
        {
            auto node = mBigslist.removeEntry();
            free(node);
        }
    }
    uintptr MemAlloctor::alloctor(uint size
#ifdef _check_memory_free
                                  ,
                                  cstring filepos, uint32 linepos
#endif
    )
    {
        // 第一步 对齐大小
        uint asize = alignup(size, (uint)_block_alig_size);

        // 超出 256 字节，从操作系统分配
        if (size > _block_max_size)
        {
            auto blk = (BlockHead *)malloc(asize + sizeof(BlockHead));
            mBigslist.insertEntry(blk);
            // 内存debug信息
#ifdef _check_memory_free
            blk->mDbgFile = filepos;
            blk->mDbgLine = linepos;
#endif
            return blk->mData;
        }
        // 可以在池中分配内存
        // 获取对应池
        auto index = (asize / _block_alig_size) - 1;
        auto &pool = mPoollist[index];
        PoolkHead *pools = nullptr;
        for (auto iter = pool.front();
             iter != pool.getRoot();
             iter = iter->getNext())
        {
            if (iter->mBlocks.getCount() > 0)
            {
                pools = iter;
                break;
            }
        }
        // 没有找到可用池，重新分配
        if (pools == nullptr)
        {
            pools = mCenter.getPool();
            pools->initPool(asize);
            pool.insertEntry(pools);
        }
        // 从池中获取内存块
        auto blk = pools->mBlocks.removeTail();
        mBlocklist[index].insertEntry(blk);
        // 内存debug信息
#ifdef _check_memory_free
        blk->mDbgFile = filepos;
        blk->mDbgLine = linepos;
#endif
        return blk->mData;
    }

    void MemAlloctor::dealloctor(uintptr blk)
    {
        // 查找在哪个池
        uint indexpool = 0;
        PoolkHead *pools = nullptr;
        uint orgaddr = (uint)blk;
        // 块头地址
        BlockHead *blkha = (BlockHead *)(orgaddr - sizeof(BlockHead));
        for (; indexpool < 32; ++indexpool)
        {
            for (auto iter = mPoollist[indexpool].getEntry();
                 iter != mPoollist[indexpool].getRoot();
                 iter = iter->getNext())
            {
                uint start = (uint)iter;
                uint end = start + _mem_pool_size;
                if (start < orgaddr && orgaddr < end)
                {
                    pools = iter;
                    break;
                }
            }
            if (pools != nullptr)
                break;
        }
        // 匹配池
        if (pools != nullptr)
        {
            mBlocklist[indexpool].remove(blkha);
            pools->mBlocks.insertEntry(blkha);
            // 判断池是否不再使用
            if (pools->mBlkCount == pools->mBlocks.getCount())
            {
                mPoollist[indexpool].remove(pools);
                mCenter.freePool(pools);
            }
            return;
        }
        // 匹配大块内存
        for (auto iter = mBigslist.getEntry();
             iter != mBigslist.getRoot();
             iter = iter->getNext())
        {
            uint start = (uint)iter;
            uint end = start + _mem_pool_size;
            if (iter == blkha)
            {
                mBigslist.remove(iter);
                free(iter);
                return;
            }
        }
        memlog("未知内存块：0x%x\n", blk);
    }

#ifdef _check_memory_free
    uintptr alloc_(uint size, cstring filepos, uint32 linepos)
#else
    uintptr alloc(uint size)
#endif
    {
        return nullptr;
    }

    void dealloc(uintptr block)
    {
    }

}