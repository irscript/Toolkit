#ifndef __AIRARRAY__H__
#define __AIRARRAY__H__
#include <airkit/Core/airTypes.h>
namespace air
{
    // 一维数组
    template <typename Type, const uint col>
    class Array
    {
    protected:
        Type mArr[col];

    public:
        inline OPT_UNROLL Array()
        {
            for (uint i = 0; i < col; ++i)
                constructor<Type>(&mArr[i]);
        }
        inline OPT_UNROLL ~Array()
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
    };
    // 二维数组
    template <typename Type, const uint col, const uint col2>
    class Array2
    {
    protected:
        Array<Type, col> mArr[col2];

    public:
        inline OPT_UNROLL Array2()
        {
            for (uint i = 0; i < col2; ++i)
                constructor<Array<Type, col>>(&mArr[i]);
        }
        inline OPT_UNROLL ~Array2()
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
    };
    // 三维数组
    template <typename Type, const uint col, const uint col2, const uint col3>
    class Array3
    {
    protected:
        Array2<Type, col, col2> mArr[col3];

    public:
        inline OPT_UNROLL Array3()
        {
            for (uint i = 0; i < col3; ++i)
                constructor<Array2<Type, col, col2>>(&mArr[i]);
        }
        inline OPT_UNROLL ~Array3()
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
    };
}

#endif //!__AIRARRAY__H__