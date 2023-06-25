#ifndef __AIRVECTOR__H__
#define __AIRVECTOR__H__
#include <airkit/Core/airAlloc.h>
#include <airkit/Struct/airHeapSort.h>
namespace air
{
    // 通用向量模板
    template <typename Type, const AlloctorType mem = AlloctorType::Thread>
    class Vector
    {
    public:
        using sizetype = uint;

        Vector() : mCapc(0), mSize(0), mData(nullptr) {}
        Vector(sizetype size) : Vector() { realloc(size); }
        ~Vector() { clear(); }

        inline sizetype size() const { return mSize; }
        inline sizetype length() const { return mSize; }
        inline Type *data() { return mData; }
        inline bool empty() const { return mSize == 0; }

        inline void clear()
        {
            if (mData != nullptr)
            {
                // 先析构
                for (sizetype i = 0; i < mSize; ++i)
                    destructor<Type>(&mData[i]);
                // 释放内存
                mAlloc.dealloc(mData);
            }
            mSize = 0;
            mData = nullptr;
        }
        // 重新分配内存大小
        inline void resize(sizetype size) { realloc(size); mSize = size;}

        // 插入
        void insert(const Type &element, sizetype index)
        {
            make_ensure(index <= mSize);

            // 防止element在数组中
            const Type ele(element);

            // 需要扩展内存
            if (mSize + 1 > mCapc)
            {
                // 重新分配内存
                sizetype size = mSize + 1 + (mCapc < 500 ? (mCapc < 5 ? 5 : mSize) : mSize >> 2);
                realloc(size);
            }
            // 移动数据
            for (sizetype i = mSize; i > index; --i)
            {
                if (i < mSize)
                    destructor<Type>(&mData[i]);
                constructor<Type, Type &>(&mData[i], mData[i - 1]);
            }
            // 添加数据
            if (mSize > index)
                destructor<Type>(&mData[index]);
            constructor<Type, const Type &>(&mData[index], ele);
            ++mSize;
        }
        // 擦除
        void erase(sizetype index)
        {
            make_ensure(index < mSize);

            // 移动数据
            for (sizetype i = index + 1; i < mSize; ++i)
            {
                destructor<Type>(&mData[i - 1]);
                constructor<Type, Type &>(&mData[i - 1], mData[i]);
            }
            --mSize;
            // 析构函数
            destructor<Type>(&mData[mSize]);
        }
        // 连续擦除
        void erase(sizetype index, sizetype count)
        {
            if (index >= mSize || count == 0)
                return;
            // 确保数量在范围
            if (index + count > mSize)
                count = mSize - index;

            sizetype i = 0;
            sizetype end = index + count;

            // 析构
            for (i = index; i < end; ++i)
                destructor<Type>(&mData[i]);

            // 移动数据
            sizetype end2 = end + mSize - end;
            for (sizetype j = index; j < end2; ++j)
                constructor<Type, Type &>(&mData[j], mData[i++]);

            mSize -= count;
        }

        inline void pushFront(const Type &elem) { insert(elem, 0); }
        inline void pushBack(const Type &elem) { insert(elem, mSize); }
        inline void popFront() { erase(0); }
        inline void popBack() { erase(mSize - 1); }

        inline Type &at(sizetype index)
        {
            make_ensure(index < mSize);
            return mData[index];
        }
        inline Type &operator[](sizetype index)
        {
            make_ensure(index < mSize);
            return mData[index];
        }

        // 排序
        inline void sort()
        {
            if (mSize > 1)
                heapsort<Type>(mData, mSize);
        }

        // 前向线性搜索
        sizetype linearSearch(const Type &elem) const
        {
            for (sizetype i = 0; i < mSize; ++i)
                if (elem == mData[i])
                    return i;
            return -1;
        }
        // 逆向线性搜索
        sizetype linearReverseSearch(const Type &elem) const
        {
            for (sizetype i = mSize - 1; i >= 0; --i)
                if (elem == mData[i])
                    return i;
            return -1;
        }
        // 折半搜索
        sizetype binarySearch(const Type &elem, sizetype start, sizetype end) const
        {
            // 验证参数是否有效
            if (mSize == 0 ||
                start > mSize ||
                end > mSize ||
                elem < mData[start] ||
                elem > mData[end])
                return -1;

            // 判断首尾，优化效率
            if (elem == mData[start])
                return start;
            if (elem == mData[end])
                return end;

            sizetype m = 0;
            while (true)
            {
                m = (start + end) >> 1;
                if (elem == mData[m])
                    return m;
                if (elem < mData[m])
                    end = m - 1;
                else
                    start = m + 1;
                if (start > end)
                    break;
            }
            return -1;
        }

    protected:
        sizetype mCapc;             // 缓存大小
        sizetype mSize;             // 真实大小
        Alloctor<Type, mem> mAlloc; // 内存分配器
        Type *mData;

        // 重新分配内存
        void realloc(sizetype size)
        {
            // 大小4字节对齐
            size = alignup<sizetype>(size, 4);
            if (size == mSize)
                return;

            Type *old = mData;
            mCapc = size;
            // 重新分配内存
            mData = mAlloc.allocArr(size
#ifdef _check_memory_free
                                    ,
                                    this_file(), this_line()
#endif
            );

            // 拷贝数据
            sizetype amount = mSize < size ? mSize : size;
            for (sizetype i = 0; i < amount; ++i)
            {
                constructor<Type, Type &>(&mData[i], old[i]);
                destructor<Type>(&old[i]);
            }
            // 析构
            for (sizetype i = amount; i < mSize; ++i)
                destructor<Type>(&old[i]);

            mSize = amount;
            // 释放内存
            if (old != nullptr)
                mAlloc.dealloc(old);
        }
    };
}
#endif //!__AIRVECTOR__H__