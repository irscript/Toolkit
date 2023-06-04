#ifndef __AIRASTRING__H__
#define __AIRASTRING__H__

#include <airkit/Core/airObject.h>

namespace air
{
    /**
     * 注意：在哪个线程中分配，就在哪个线程释放
     * 建议在主线程分配
     */

    // 只读哈希字符串
    struct AString
    {
        uint16 mSize;  // 字符串长度
        uint16 mHash;  // 字符哈希
        uint8 mData[]; // 字符串内容

        // 释放内存
        inline void release() { tdealloc(this); }

        // 生成字符串
        static const AString *make(cstring szStr);
    };

}
#endif //!__AIRASTRING__H__