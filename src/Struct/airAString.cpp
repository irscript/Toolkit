
#include <airkit/Struct/airAString.h>
#include <airkit/Hash/airHash32.h>

namespace air
{
    const AString *AString::make(cstring szStr)
    {
        make_ensure(szStr);
        // 计算哈希值
        auto length = strlen(szStr);
        auto hash = Hash::ELF((uint8 *)szStr, length);
        // 分配内存
        auto ret = (AString *)alloc(length + sizeof(AString));
        // 初始化内存
        ret->mSize = length;
        ret->mHash = hash;
        mempcpy(ret->mData, szStr, length);
        return ret;
    }
}
