#ifndef __AIRBITS__H__
#define __AIRBITS__H__
#include <airkit/Core/airTypes.h>
namespace air
{

    template <const uint bits>
    struct Bits
    {
        enum
        {
            BitsCnt = alignup<uint>(bits, 8)
        };
        uint8 mByte[BitsCnt];
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