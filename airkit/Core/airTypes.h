#ifndef __AIRTYPES__H__
#define __AIRTYPES__H__

#include <cstdint>
#include <new>
#include <airkit/AIR_COMPILE_OPTION.h>
namespace air
{
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using sint = ssize_t;
    using uint = size_t;

    using flt32 = float;
    using flt64 = double;

    using cstring = const char *;
    using uintptr = void *;

    enum class EBool : uint32
    {
        False,
        True
    };

    // 向上大小对齐
    template <typename T>
    inline T alignup(T num, T base) { return ((num + base - 1) & (~(base - 1))); }

//-------通用宏定义
#define this_file() __FILE__
#define this_line() __LINE__
#define this_func() __FUNCTION__
// 求数组维度
#define array_size(obj, type) (sizeof(obj) / sizeof(type))
// 位左偏移
#define bits32(at) (1u << at)
// 位左偏移
#define bits64(at) (1ull << at)
}

#endif //!__AIRTYPES__H__