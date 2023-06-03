#ifndef __AIRBITS__H__
#define __AIRBITS__H__
#include <airkit/Core/airTypes.h>
namespace air
{
    // 固定位集合
    template <const uint bits>
    struct Bitset
    {
        enum
        {
            BitsCnt = alignup<uint>(bits, 8), // 位对齐
            ByteCcnt = BitsCnt / 8,           // 字节数
        };

        Bitset() { memset(&mByte, 0, ByteCcnt); }
        ~Bitset() {}

        void set(uint index)
        {
            make_ensure(index < BitsCnt);
            const uint byte = index / 8;
            const uint bit = index % 8;
            mByte[byte] |= (1 << bit);
        }
        void unset(uint index)
        {
            make_ensure(index < BitsCnt);
            const uint byte = index / 8;
            const uint bit = index % 8;
            mByte[byte] &= ~(1 << bit);
        }
        uint get(uint index)
        {
            make_ensure(index < BitsCnt);
            const uint byte = index / 8;
            const uint bit = index % 8;
            return mByte[byte] & (1 << bit);
        }

    protected:
        uint8 mByte[ByteCcnt];
    };

    // 位转换
    struct BitsCast32
    {
        union
        {
            int32 mI32;
            uint32 mU32;
            flt32 mF32;
        };
        inline flt32 toFlt32(int32 value)
        {
            mI32 = value;
            return mF32;
        }
        inline flt32 toFlt32(uint32 value)
        {
            mU32 = value;
            return mF32;
        }
        inline int32 toInt32(flt32 value)
        {
            mF32 = value;
            return mI32;
        }
        inline uint32 toUInt32(flt32 value)
        {
            mF32 = value;
            return mU32;
        }
        inline bool sign() const { return (mU32 >> 31) != 0; }
    };
    struct BitsCast64
    {
        union
        {
            int64 mI64;
            uint64 mU64;
            flt64 mF64;
        };
        inline flt64 toFlt64(int64 value)
        {
            mI64 = value;
            return mF64;
        }
        inline flt64 toFlt64(uint64 value)
        {
            mU64 = value;
            return mF64;
        }
        inline int64 toInt64(flt64 value)
        {
            mF64 = value;
            return mI64;
        }
        inline uint64 toUInt64(flt64 value)
        {
            mF64 = value;
            return mU64;
        }
        inline bool sign() const { return (mU64 >> 63) != 0; }
    };
}
#endif //!__AIRBITS__H__