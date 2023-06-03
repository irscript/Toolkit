#ifndef __AIROBJECT__H__
#define __AIROBJECT__H__

#include <airkit/Core/airAlloc.h>
#include <airkit/Plat/airAtomic.h>

namespace air
{
    // 引用计数对象
    class IRefObject
    {
    private:
        volatile int32 mRefCnt; // 引用计数
    public:
        IRefObject() : mRefCnt(1) {}
        virtual ~IRefObject() = 0;
        // 引用一次
        inline void grab() { Atomic<int32>::addFetch(mRefCnt, 1); }
        // 释放一次
        inline void drop()
        {
            Atomic<int32>::subFetch(mRefCnt, 1);
            if (mRefCnt == 0)
            {
                destructor(this);
                dealloc(this);
            }
        }
    };

    // 引用对象持有者模板
    template <typename Type>
    class Holder
    {
    private:
        Type *mRefObj;

    public:
        Holder() = delete;
        inline Holder(Type *ref) : mRefObj(ref)
        {
            if (ref != nullptr)
                mRefObj->grab();
        }
        inline Holder(Holder &ref) : mRefObj(ref.mRefObj)
        {
            if (mRefObj != nullptr)
                mRefObj->grab();
        }
        inline ~Holder()
        {
            if (mRefObj != nullptr)
                mRefObj->drop();
        }
        // 重载操作符
        inline Holder &operator=(Holder &rhs)
        {
            if (mRefObj != nullptr)
                mRefObj->drop();
            mRefObj = rhs.mRefObj;
            if (mRefObj != nullptr)
                mRefObj->grab();
        }
        inline Type &operator*() { return *mRefObj; }
        inline Type *operator->() { return mRefObj; }
    };

    // 生成模板
    template <typename Type>
    inline Holder<Type> make_holder(Type *obj) { return Holder<Type>(obj); }

    template <typename Type>
    inline Holder<Type> make_holder(Type &obj) { return Holder<Type>(&obj); }
}
#endif //!__AIROBJECT__H__