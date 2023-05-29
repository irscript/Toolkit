#ifndef __AIRSTRING__H__
#define __AIRSTRING__H__

#include <airkit/Core/airObject.h>
namespace air
{
    

    template <typename Type>
    class TString : public IRefObject
    {
    public:
        using sizetype = uint32;
        TString() : mCapc(0), mSize(0), mData(nullptr) {}

        inline sizetype size() const { return mSize; }
        inline sizetype length() const { return mSize; }

    protected:
        sizetype mCapc; // 缓存大小
        sizetype mSize; // 真实大小
        Type *mData;
    };

    // 多字节字符串
    using String = TString<uint8>;
    using StringHolder = Holder<String>;
    // 宽字符串
    using WString = TString<uint16>;
    using WStringHolder = Holder<WString>;

    // 字符串视图
}
#endif //!__AIRSTRING__H__