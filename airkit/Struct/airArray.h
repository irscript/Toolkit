#ifndef __AIRARRAY__H__
#define __AIRARRAY__H__
#include <airkit/Core/airTypes.h>
namespace air
{
    // 一维数组
    template <typename Type, const uint col>
    class Array
    {
    public:
        // 迭代器
        class Iterator
        {
        private:
            friend class Array;
            uint mIndex;
            explicit Iterator(const uint index = -1) : mIndex(index) {}

        public:
            Iterator(const Iterator &rhs) : mIndex(rhs.mIndex) {}

            // 是否有效
            bool valid() const { return mIndex < col; }

            // 重载操作符
            // 前置 ++、--
            inline Iterator &operator++()
            {
                ++mIndex;
                return *this;
            }
            inline Iterator &operator--()
            {
                --mIndex;
                return *this;
            }
            // 后置 ++、--
            inline Iterator operator++(int32)
            {
                Iterator ret = *this;
                ++mIndex;
                return ret;
            }
            inline Iterator operator--(int32)
            {
                Iterator ret = *this;
                --mIndex;
                return ret;
            }
            inline Iterator &operator=(const Iterator &rhs)
            {
                mIndex = rhs.mIndex;
                return *this;
            }

            inline bool operator==(const Iterator &rhs) { return mIndex == rhs.mIndex; }
            inline bool operator!=(const Iterator &rhs) { return mIndex != rhs.mIndex; }

            inline uint operator*() { return mIndex; }
        };

    protected:
        Type mArr[col];

    public:
        inline opt_unroll Array()
        {
            for (uint i = 0; i < col; ++i)
                constructor<Type>(&mArr[i]);
        }
        inline opt_unroll ~Array()
        {
            for (uint i = 0; i < col; ++i)
                destructor<Type>(&mArr[i]);
        }

        inline uint getCol() const { return col; }

        inline Type &operator[](uint index)
        {
            make_ensure(index < col);
            return mArr[index];
        }
        inline Iterator begin() { return Iterator(0); }
        inline Iterator end() { return Iterator(col); }
        inline Type &operator[](const Iterator &iter)
        {
            make_ensure(iter.mIndex < col);
            return mArr[iter.mIndex];
        }
    };
    // 二维数组
    template <typename Type, const uint col, const uint col2>
    class Array2
    {
    public:
        // 迭代器
        class Iterator
        {
        private:
            friend class Array2;
            uint mI, mJ;
            explicit Iterator(const uint i = col, const uint j = col2) : mI(i), mJ(j) {}

        public:
            Iterator(const Iterator &rhs) : mI(rhs.mI), mJ(rhs.mJ) {}

            // 是否有效
            bool valid() const { return mI < col && mJ < col2; }

            const uint i() const { return mI; }
            const uint j() const { return mJ; }

            // 重载操作符
            // 前置 ++、--
            inline Iterator &operator++()
            {
                ++mJ;
                if (mJ == col2)
                {
                    mJ = 0;
                    ++mI;
                }
                return *this;
            }
            inline Iterator &operator--()
            {
                --mJ;
                if (mJ == 0)
                {
                    mJ = col2 - 1;
                    --mI;
                }
                return *this;
            }
            // 后置 ++、--
            inline Iterator operator++(int32)
            {
                Iterator ret = *this;
                ++mJ;
                if (mJ == col2)
                {
                    mJ = 0;
                    ++mI;
                }
                return ret;
            }
            inline Iterator operator--(int32)
            {
                Iterator ret = *this;
                --mJ;
                if (mJ == 0)
                {
                    mJ = col2 - 1;
                    --mI;
                }
                return ret;
            }
            inline Iterator &operator=(const Iterator &rhs)
            {
                mI = rhs.mI;
                mJ = rhs.mJ;
                return *this;
            }

            inline bool operator==(const Iterator &rhs) { return mI == rhs.mI && mJ == rhs.mJ; }
            inline bool operator!=(const Iterator &rhs) { return mI != rhs.mI || mJ != rhs.mJ; }
        };

    protected:
        Array<Type, col> mArr[col2];

    public:
        inline opt_unroll Array2()
        {
            for (uint i = 0; i < col2; ++i)
                constructor<Array<Type, col>>(&mArr[i]);
        }
        inline opt_unroll ~Array2()
        {
            for (uint i = 0; i < col2; ++i)
                destructor<Array<Type, col>>(&mArr[i]);
        }
        inline uint getCol() const { return col; }
        inline uint getCol2() const { return col2; }

        inline Array<Type, col> &operator[](uint index)
        {
            make_ensure(index < col2);
            return mArr[index];
        }
        inline Iterator begin() { return Iterator(0, 0); }
        inline Iterator end() { return Iterator(col, 0); }
        inline Type &operator[](const Iterator &iter)
        {
            make_ensure(iter.valid());
            return mArr[iter.mI][iter.mJ];
        }
    };
    // 三维数组
    template <typename Type, const uint col, const uint col2, const uint col3>
    class Array3
    {
    public:
        // 迭代器
        class Iterator
        {
        private:
            friend class Array3;
            uint mI, mJ, mK;
            explicit Iterator(const uint i = col, const uint j = col2, const uint k = col3) : mI(i), mJ(j), mK(k) {}

        public:
            Iterator(const Iterator &rhs) : mI(rhs.mI), mJ(rhs.mJ), mK(rhs.mK) {}

            // 是否有效
            bool valid() const { return mI < col && mJ < col2 && mK < col3; }

            const uint i() const { return mI; }
            const uint j() const { return mJ; }
            const uint k() const { return mK; }

            // 重载操作符
            // 前置 ++、--
            inline Iterator &operator++()
            {
                ++mK;
                if (mK == col3)
                {
                    mK = 0;
                    ++mJ;
                    if (mJ == col2)
                    {
                        mJ = 0;
                        ++mI;
                    }
                }
                return *this;
            }
            inline Iterator &operator--()
            {
                -mK;
                if (mK == 0)
                {
                    mK = col3 - 1;
                    ++mJ;
                    if (mJ == 0)
                    {
                        mJ = col2 - 1;
                        ++mI;
                    }
                }
                return *this;
            }
            // 后置 ++、--
            inline Iterator operator++(int32)
            {
                Iterator ret = *this;
                ++mK;
                if (mK == col3)
                {
                    mK = 0;
                    ++mJ;
                    if (mJ == col2)
                    {
                        mJ = 0;
                        ++mI;
                    }
                }
                return ret;
            }
            inline Iterator operator--(int32)
            {
                Iterator ret = *this;
                -mK;
                if (mK == 0)
                {
                    mK = col3 - 1;
                    ++mJ;
                    if (mJ == 0)
                    {
                        mJ = col2 - 1;
                        ++mI;
                    }
                }
                return ret;
            }
            inline Iterator &operator=(const Iterator &rhs)
            {
                mI = rhs.mI;
                mJ = rhs.mJ;
                mK = rhs.mK;
                return *this;
            }

            inline bool operator==(const Iterator &rhs) { return mI == rhs.mI && mJ == rhs.mJ && mK == rhs.mK; }
            inline bool operator!=(const Iterator &rhs) { return mI != rhs.mI || mJ != rhs.mJ || mK != rhs.mK; }
        };

    protected:
        Array2<Type, col, col2> mArr[col3];

    public:
        inline opt_unroll Array3()
        {
            for (uint i = 0; i < col3; ++i)
                constructor<Array2<Type, col, col2>>(&mArr[i]);
        }
        inline opt_unroll ~Array3()
        {
            for (uint i = 0; i < col3; ++i)
                destructor<Array2<Type, col, col2>>(&mArr[i]);
        }
        inline uint getCol() const { return col; }
        inline uint getCol2() const { return col2; }
        inline uint getCol3() const { return col3; }

        inline Array2<Type, col, col2> &operator[](uint index)
        {
            make_ensure(index < col3);
            return mArr[index];
        }

        inline Iterator begin() { return Iterator(0, 0, 0); }
        inline Iterator end() { return Iterator(col, 0, 0); }
        inline Type &operator[](const Iterator &iter)
        {
            make_ensure(iter.valid());
            return mArr[iter.mI][iter.mJ][iter.mK];
        }
    };
}

#endif //!__AIRARRAY__H__