#include <airkit/Core/airAlloc.h>
#include <airkit/Struct/airIList.h>
#include <airkit/Plat/airLoger.h>
#include <stdlib.h>
#include <stdio.h>

namespace air
{
    void assertion(cstring szExp, cstring szFile, cstring szFunc, int line)
    {
        // 跟踪日志系统
        errlog("Assertion failed!\n\texp: %s\n\tfile:%s\n\tfunc: %s\n\tline: %d\n", szExp, szFile, szFunc, line);
        exit(-1);
    }
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
        inline void initPool(uint32 size);
        uint32 mBlkSize;          // 池内单个分块大小
        uint32 mBlkCount;         // 池内分块个数
        IList<BlockHead> mBlocks; // 未使用块链表
    };
    // 巨页内存头
    struct AreaHead : public IListNode<AreaHead>
    {
        // 初始化页
        inline void initArea();
        uint32 mPoolCount;       // 页内分池个数
        IList<PoolkHead> mPools; // 未使用池链表
    };
    // 内存池分配中心
    struct MemPoolCenter
    {
        inline MemPoolCenter() { init(); }
        // 初始化
        inline void init() { mArealist.initList(); }
        // 终止化
        inline void terminal();
        // 获取一个页
        inline AreaHead *getArea();
        // 获取一个池
        inline PoolkHead *getPool();
        // 释放一个池
        inline void freePool(PoolkHead *pool);

    private:
        Spinlock mLock;
        IList<AreaHead> mArealist; // 巨页链表
    };

    // 通用内存分配器
    struct MemAlloctor : public IAlloctor
    {
        inline MemAlloctor() {}
        inline ~MemAlloctor() {}

        static MemPoolCenter mCenter;             // 内存池分配中心
        IList<PoolkHead> mPoollist[_pool_count];  // 使用的池链表
        IList<BlockHead> mBlocklist[_pool_count]; // 使用的块链表
        IList<BlockHead> mBigslist;               // 大内存块

        inline void init();
        inline void terminal();

        inline void checkFree();

        uintptr alloctor(
            uint size
#ifdef _check_memory_free
            ,
            cstring filepos, uint32 linepos
#endif
            ) override;
        void dealloctor(uintptr blk) override;
    };

    MemPoolCenter MemAlloctor::mCenter;

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
        mLock.lock();
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
        mLock.unlock();
        return pool;
    }
    void MemPoolCenter::freePool(PoolkHead *pool)
    {
        if (pool == nullptr)
            return;

        mLock.lock();
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
                // 再次初始化池
                pool->mBlocks.initList();
                pool->mBlkSize = 0;
                pool->mBlkCount = 0;
                // 页是否使用完毕,使用完毕释放内存
                if (iter->mPoolCount == iter->mPools.getCount())
                {
                    mArealist.remove(iter);
                    free(iter);
                }

                mLock.unlock();
                return;
            }
        }
        memlog("未知池：0x%lx\n", pool);
        mLock.unlock();
    }

    void MemAlloctor::init()
    {
        for (uint32 i = 0; i < _pool_count; ++i)
        {
            mPoollist[i].initList();
            mBlocklist[i].initList();
        }
        mBigslist.initList();
    }

    void MemAlloctor::checkFree()
    {
#ifdef _check_memory_free
        uint dbgcnt = 0;
        memlog.start();
        auto id = Thread::getCurrentThreadID();
        // memlog.print2("");
        memlog.append("\n\n------------------checking-free-start[ thread:0x%lx ]---------------------\n", id);

        // 通用内存块
        for (uint i = 0; i < _pool_count; ++i)
        {
            auto size = (i + 1) * 8;
            for (auto iter = mBlocklist[i].getEntry();
                 iter != mBlocklist[i].getRoot();
                 iter = iter->getNext())
            {
                memlog.append("checking-free>\taddr: 0x%lx\tsize: %d \tin [file: %s\tline: %d]!\n",
                              iter, size,
                              iter->mDbgFile, iter->mDbgLine);
                ++dbgcnt;
            }
        }
        // memlog.append("\n------------------checking-big-free----------------------\n");
        //  大内存
        for (auto iter = mBigslist.getEntry();
             iter != mBigslist.getRoot();
             iter = iter->getNext())
        {
            memlog.append("checking-big-free>\taddr: 0x%lx\tin [file: %s\tline: %d ]!\n",
                          iter,
                          iter->mDbgFile, iter->mDbgLine);
            ++dbgcnt;
        }
        memlog.append("\nchecking-free-count: %ld\n\n", dbgcnt);
        memlog.append("------------------checking-free-end[ thread:0x%lx ]----------------------\n\n\n", id);
        memlog.end();
#endif
    }

    void MemAlloctor::terminal()
    {
        // 释放内存池
        for (uint i = 0; i < _pool_count; ++i)
        {
            auto cnt = mPoollist[i].getCount();
            for (uint ic = 0; ic < cnt; ++ic)
            {
                auto pool = mPoollist[i].removeEntry();
                mCenter.freePool(pool);
            }
            mBlocklist[i].initList();
        }
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
        if (blk == nullptr)
            return;
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

    // 内存池线程局部变量
    class MemAlloctorTLS
    {
    public:
        MemAlloctorTLS() {}

        inline void init() { mInstance.create(MemAlloctorTLS::destroy); }
        inline void terminal() { mInstance.destroy(); }

        inline MemAlloctor &instance()
        {
            MemAlloctor *obj = (MemAlloctor *)mInstance.get();
            if (obj == nullptr)
            {
                obj = (MemAlloctor *)malloc(sizeof(MemAlloctor));
                constructor<MemAlloctor>(obj);
                obj->init();
                mInstance.set(obj);
            }
            return *obj;
        }
        // 唯一实例
        static MemAlloctorTLS mTLS;

    private:
        TLSV mInstance;

        MemAlloctorTLS(const MemAlloctorTLS &) = delete;
        MemAlloctorTLS &operator=(const MemAlloctorTLS &) = delete;
        // 内存释放
        static void destroy(uintptr ins);
    };
    MemAlloctorTLS MemAlloctorTLS::mTLS;
    void MemAlloctorTLS::destroy(uintptr ins)
    {
        if (ins != nullptr)
        {
            MemAlloctor *obj = (MemAlloctor *)ins;
            obj->terminal();
            destructor<MemAlloctor>(obj);
            free(obj);
        }
    }

    struct OSMemAlloctor : public IAlloctor
    {
        uintptr alloctor(
            uint size
#ifdef _check_memory_free
            ,
            cstring filepos, uint32 linepos
#endif
            ) override;
        void dealloctor(uintptr blk) override;

        static OSMemAlloctor mInstance;
    };
    OSMemAlloctor OSMemAlloctor ::mInstance;
    uintptr OSMemAlloctor ::alloctor(uint size
#ifdef _check_memory_free
                                     ,
                                     cstring filepos, uint32 linepos
#endif
    )
    {
        return malloc(size);
    }
    void OSMemAlloctor ::dealloctor(uintptr blk) { free(blk); }
    // 多线程分配器
    struct MuitlMemAlloctor : public MemAlloctor
    {
        MuitlMemAlloctor() {}
        ~MuitlMemAlloctor() {}

        Spinlock mLock; // 分配释放锁

        inline void init();
        inline void terminal();

        inline void checkFree();

        uintptr alloctor(
            uint size
#ifdef _check_memory_free
            ,
            cstring filepos, uint32 linepos
#endif
            ) override;
        void dealloctor(uintptr blk) override;

        static MuitlMemAlloctor mInstance;
    };

    MuitlMemAlloctor MuitlMemAlloctor::mInstance;

    inline void MuitlMemAlloctor::init()
    {
        Spinlocker lock(mLock);
        MemAlloctor::init();
    }
    inline void MuitlMemAlloctor::terminal()
    {
        Spinlocker lock(mLock);
        MemAlloctor::terminal();
    }

    inline void MuitlMemAlloctor::checkFree()
    {
        Spinlocker lock(mLock);
#ifdef _check_memory_free
        uint dbgcnt = 0;
        memlog.start();
        memlog.append("\n\n------------------checking-free-start[ shared ]---------------------\n");

        // 通用内存块
        for (uint i = 0; i < _pool_count; ++i)
        {
            auto size = (i + 1) * 8;
            for (auto iter = mBlocklist[i].getEntry();
                 iter != mBlocklist[i].getRoot();
                 iter = iter->getNext())
            {
                memlog.append("checking-free>\taddr: 0x%lx\tsize: %d \tin [file: %s\tline: %d]!\n",
                              iter, size,
                              iter->mDbgFile, iter->mDbgLine);
                ++dbgcnt;
            }
        }
        // memlog.append("\n------------------checking-big-free----------------------\n");
        //  大内存
        for (auto iter = mBigslist.getEntry();
             iter != mBigslist.getRoot();
             iter = iter->getNext())
        {
            memlog.append("checking-big-free>\taddr: 0x%lx\tin [file: %s\tline: %d ]!\n",
                          iter,
                          iter->mDbgFile, iter->mDbgLine);
            ++dbgcnt;
        }
        memlog.append("\nchecking-free-count: %ld\n\n", dbgcnt);
        memlog.append("------------------checking-free-end[ shared ]----------------------\n\n\n");
        memlog.end();
#endif
    }

    uintptr MuitlMemAlloctor::alloctor(
        uint size
#ifdef _check_memory_free
        ,
        cstring filepos, uint32 linepos
#endif
    )
    {
        Spinlocker lock(mLock);
        return MemAlloctor::alloctor(size
#ifdef _check_memory_free
                                     ,
                                     filepos, linepos
#endif
        );
    }
    void MuitlMemAlloctor::dealloctor(uintptr blk)
    {
        if (blk == nullptr)
            return;
        Spinlocker lock(mLock);
        MemAlloctor::dealloctor(blk);
    }

    // 初始化内存系统
    void initMemSys()
    {
        MemAlloctor::mCenter.init();
        MuitlMemAlloctor::mInstance.init();
        MemAlloctorTLS::mTLS.init();
    }
    // 释放内存系统
    void terminalMemSys()
    {
        // 多线程分配器内存释放情况
        MuitlMemAlloctor::mInstance.checkFree();
        MuitlMemAlloctor::mInstance.terminal();
        checkMemSys(); // 主线程内存释放情况
        MemAlloctorTLS::mTLS.terminal();
        MemAlloctor::mCenter.terminal();
    }
    IAlloctor &getThreadAlloctor()
    {
        return MemAlloctorTLS::mTLS.instance();
    }
    IAlloctor &getSharedAlloctor()
    {
        return MuitlMemAlloctor::mInstance;
    }
    IAlloctor &getOSAlloctor()
    {
        return OSMemAlloctor ::mInstance;
    };
    void checkMemSys()
    {
        auto &memsys = MemAlloctorTLS::mTLS.instance();
        memsys.checkFree();
    }
}