#ifndef __AIRMATHDEF__H__
#define __AIRMATHDEF__H__
#include <airkit/Core/airTypes.h>
namespace air
{
    // 基本数学函数
    namespace Math
    {
        // 常量定义
        constexpr int8 cInt8Max = 127;
        constexpr int8 cInt8Min = -128;
        constexpr uint8 cUInt8Max = 0xFF;
        constexpr uint8 cUInt8Min = 0;

        constexpr int16 cInt16Max = 32767;
        constexpr int16 cInt16Min = -32768;
        constexpr uint16 cUInt16Max = 0xFFFF;
        constexpr uint16 cUInt16Min = 0;

        constexpr int32 cInt32Max = 2147483647;
        constexpr int32 cInt32Min = -2147483648;
        constexpr uint32 cUInt32Max = 0xFFFFFFFF;
        constexpr uint32 cUInt32Min = 0;

        constexpr int64 cInt64Max = 9223372036854775807;
        constexpr int64 cInt64Min = -(9223372036854775808ull);
        constexpr uint64 cUInt64Max = 0xFFFFFFFFFFFFFFFF;
        constexpr uint64 cUInt64Min = 0;

        constexpr flt32 cFlt32Max = 3.402823466E+38F;
        constexpr flt32 cFlt32Min = 1.17549435e-38F;

        constexpr flt64 cFlt64Max = 1.7976931348623158e+308;
        constexpr flt64 cFlt64Min = 2.2250738585072014e-308;

        // 浮点判断误差控制
        constexpr flt32 cFlt32Epsilon = 1.192092896e-07F;
        constexpr flt64 cFlt64Epsilon = 2.2204460492503131e-016;

#define cRealEpsilon cFlt32Epsilon

        // PI相关定义
        constexpr flt32 cFlt32PI = 3.14159265359f;
        constexpr flt32 cFlt32PIInv = 1.0f / cFlt32PI;
        constexpr flt32 cFlt32PIHalf = cFlt32PI / 2.0f;

        constexpr flt64 cFlt64PI = 3.1415926535897932384626433832795028841971693993751;
        constexpr flt64 cFlt64PIInv = 1.0f / cFlt64PI;
        constexpr flt64 cFlt64PIHalf = cFlt64PI / 2.0f;

        // 角度和弧度转换
        constexpr flt32 cFlt32Deg2Rad = cFlt32PI / 180.f;
        constexpr flt32 cFlt32Rad2Deg = 180.0f / cFlt32PI;

        constexpr flt64 cFlt64Deg2Rad = cFlt64PI / 180.f;
        constexpr flt64 cFlt64Rad2Deg = 180.0f / cFlt64PI;

        inline constexpr flt32 rad2deg(flt32 radins) { return cFlt32Rad2Deg * radins; }
        inline constexpr flt64 rad2deg(flt64 radins) { return cFlt64Rad2Deg * radins; }

        inline constexpr flt32 deg2rad(flt32 degress) { return cFlt32Deg2Rad * degress; }
        inline constexpr flt64 deg2rad(flt64 degress) { return cFlt64Deg2Rad * degress; }

        // 基本类型等于判断
        template <typename Type>
        inline Bool equals(Type left, Type right) { return left == right ? Bool::True : Bool::False; }

        // 浮点特化
        template <>
        inline Bool equals(flt32 left, flt32 right) { return (left + cFlt32Epsilon >= right) && (left - cFlt32Epsilon <= right) ? Bool::True : Bool::False; }
        template <>
        inline Bool equals(flt64 left, flt64 right) { return (left + cFlt64Epsilon >= right) && (left - cFlt64Epsilon <= right) ? Bool::True : Bool::False; }

        // 获取最小值
        template <typename Type>
        inline const Type &min(const Type &a, const Type &b) { return a < b ? a : b; }
        template <typename Type>
        inline const Type &min(const Type &a, const Type &b, const Type &c) { return a < b ? min(a, c) : min(b, c); }
        // 获取最大值
        template <typename Type>
        inline const Type &max(const Type &a, const Type &b) { return a > b ? a : b; }
        template <typename Type>
        inline const Type &max(const Type &a, const Type &b, const Type &c) { return a > b ? max(a, c) : max(b, c); }

        // 绝对值
        template <typename Type>
        inline Type abs(const Type &a) { return a < (Type)0 ? -a : a; }

        // 返回 a 和 b 的线性插值,t=[0,1]
        template <typename Type>
        inline Type lerp(const Type &a, const Type &b, const flt32 t) { return (Type)(a * (1.0f - t) + (b * t)); }

        // 将 value 夹 在 low 和 high 之间的值
        template <typename Type>
        inline Type clamp(const Type &value, const Type &low, const Type &high) { return min(max(value, low), high); }

        // 交换
        template <typename Type, typename Type2>
        inline void swap(Type &a, Type2 &b)
        {
            Type c(a);
            a = b;
            b = c;
        }

        inline flt32 sqrt(flt32 value) { return ::sqrtf(value); }
        inline flt64 sqrt(flt64 value) { return ::sqrt(value); }

        // 三角函数
        //  flt32
        inline flt32 sin(flt32 value) { return ::sinf(value); }
        inline flt32 cos(flt32 value) { return ::cosf(value); }
        inline flt32 tan(flt32 value) { return ::tanf(value); }

        inline flt32 asin(flt32 value) { return ::asinf(value); }
        inline flt32 acos(flt32 value) { return ::acosf(value); }
        inline flt32 atan(flt32 value) { return ::atanf(value); }

        inline flt32 atan2(flt32 x, flt32 y) { return ::atan2f(x, y); }
        inline flt32 ceil(flt32 value) { return ::ceilf(value); }
        inline flt32 floor(flt32 value) { return ::floorf(value); }
        inline flt32 pow(flt32 x, flt32 y) { return ::powf(x, y); }

        inline flt32 fmod(flt32 x, flt32 y) { return ::fmodf(x, y); }
        inline flt32 flog(flt32 value) { return ::logf(value); }
        inline flt32 fabs(flt32 value) { return ::fabsf(value); }

        // flt64
        inline flt64 sin(flt64 value) { return ::sin(value); }
        inline flt64 cos(flt64 value) { return ::cos(value); }
        inline flt64 tan(flt64 value) { return ::tan(value); }

        inline flt64 asin(flt64 value) { return ::asin(value); }
        inline flt64 acos(flt64 value) { return ::acos(value); }
        inline flt64 atan(flt64 value) { return ::atan(value); }

        inline flt64 atan2(flt64 x, flt64 y) { return ::atan2(x, y); }
        inline flt64 ceil(flt64 value) { return ::ceil(value); }
        inline flt64 floor(flt64 value) { return ::floor(value); }
        inline flt64 pow(flt64 x, flt64 y) { return ::pow(x, y); }

        inline flt64 fmod(flt64 x, flt64 y) { return ::fmod(x, y); }
        inline flt64 flog(flt64 value) { return ::log(value); }
        inline flt64 fabs(flt64 value) { return ::fabs(value); }
    }
}
#endif //!__AIRMATHDEF__H__