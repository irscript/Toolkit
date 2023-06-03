#ifndef __AIRTYPES__H__
#define __AIRTYPES__H__

#include <airkit/AIR_COMPILE_OPTION.h>
#include <airkit/AIR_PCH.h>

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

    using real = flt32;

    using cstring = const char *;
    using uintptr = void *;

    enum class Bool : uint32
    {
        False,
        True
    };

    // 不可复制对象
    class NonCopyable
    {
    public:
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;
    };

    // c++构造函数调用
    template <typename Type, typename... Args>
    inline Type *constructor(Type *obj, Args... args)
    {
        Type *ret = (Type *)obj;
        ::new (obj) Type(args...);
        return ret;
    }
    // c++析构函数手动调用
    template <typename Type>
    inline void destructor(Type *obj) { obj->~Type(); }
    // 向上大小对齐
    template <typename T>
    inline constexpr T alignup(T num, T base) { return (num + base - 1) & (~(base - 1)); }
    // 向下大小对齐
    template <typename T>
    inline constexpr T aligndown(T num, T base) { return (num & ~(base - 1)); }

    // 生成ID
    inline constexpr uint16 makeID16(uint32 b1, uint32 b2) { return b1 << 8 | b2; }
    inline constexpr uint32 makeID32(uint32 b1, uint32 b2, uint32 b3, uint32 b4) { return b1 << 24 | b2 << 16 | b3 << 8 | b4; }
    inline constexpr uint64 makeID64(uint64 b1, uint64 b2, uint64 b3, uint64 b4,
                                     uint64 b5, uint64 b6, uint64 b7, uint64 b8) { return b1 << 56 | b2 << 48 | b3 << 40 | b4 << 36 |
                                                                                          b5 << 24 | b6 << 16 | b7 << 8 | b8; }
    // 位左移
    inline constexpr uint32 bits32(uint32 offset) { return 1u << offset; }
    inline constexpr uint64 bits64(uint32 offset) { return 1ull << offset; }

//-------通用宏定义
#define this_file() __builtin_FILE()
#define this_line() __LINE__
#define this_func() __PRETTY_FUNCTION__

// 求数组维度
#define array_size(obj, type) (sizeof(obj) / sizeof(type))

}

#endif //!__AIRTYPES__H__