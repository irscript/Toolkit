#ifndef __AIRSTRING__H__
#define __AIRSTRING__H__

#include <airkit/Core/airObject.h>
namespace air
{

    // 字符判断函数
    namespace CharFunc
    {
        // 转为大写字母
        inline constexpr uint32 toUpper(uint32 x) { return x >= 'a' && x <= 'z' ? x + ('A' - 'a') : x; }
        // 转为小写字母
        inline constexpr uint32 toLower(uint32 x) { return x >= 'A' && x <= 'Z' ? x + 0x20 : x; }
        // 二进制数字
        inline constexpr bool isBin(uint32 x) { return x == '0' || x == '1'; }
        // 八进制数字
        inline constexpr bool isOct(uint32 x) { return x >= '0' && x < '8'; }
        // 十进制数字
        inline constexpr bool isDec(uint32 x) { return x >= '0' && x <= '9'; }
        // 十六进制字母
        inline constexpr bool isHex(uint32 x) { return (x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F'); }
        // 是否为数字
        inline constexpr bool isNumber(uint32 x) { return x >= '0' && x <= '9'; }
        // 是否是字母
        inline constexpr bool isLetter(uint32 x) { return (x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F'); }
        // 是否是标识符字符
        inline constexpr bool isIdentifier(uint32 x) { return x == '_' || isNumber(x) || isLetter(x); }
        // 是否是字母或者数字
        inline constexpr bool isAlnum(uint32 x) { return isNumber(x) || isLetter(x); }
        // 是否是可打印字符
        inline constexpr bool isPrint(uint32 x) { return ::isprint(x); }
    }

    template <typename Type, const AlloctorType mem = AlloctorType::OS>
    class TString
    {
    public:
        // using sizetype = uint32;
        using sizetype = uint;
        TString() : mCapc(0), mSize(0), mData(nullptr) {}
        ~TString() { clear(); }
        TString(const TString &other) : TString() { *this = other; }
        TString(const Type *const other) : TString() { *this = other; }

        explicit TString(const int32 num) : TString()
        {
        }

        explicit TString(const flt32 flt) : TString()
        {
            char tmpbuf[256];
            snprintf((char *)tmpbuf, 256, "%0.6f", flt);
            *this = tmpbuf;
        }
        explicit TString(const flt64 flt) : TString()
        {
            char tmpbuf[256];
            snprintf((char *)tmpbuf, 256, "%0.6f", flt);
            *this = tmpbuf;
        }

        inline sizetype size() const { return mSize; }
        inline sizetype length() const { return mSize; }
        inline const Type *cstr() const { return mData == nullptr ? (const Type *)("\0\0\0\0") : mData; }
        inline Type *data() { return mData; }
        inline bool empty() const { return mSize == 0; }

        inline void clear()
        {
            if (mData != nullptr)
                mAlloc.dealloc(mData);
            mData = nullptr;
            mCapc = 0;
            mSize = 0;
        }

        inline TString &tolower()
        {
            for (sizetype i = 0; i < mSize; ++i)
                mData[i] = CharFunc::toLower(mData[i]);
            return *this;
        }
        inline TString &toupper()
        {
            for (sizetype i = 0; i < mSize; ++i)
                mData[i] = CharFunc::toUpper(mData[i]);
            return *this;
        }

        inline bool equalsIgnoreCase(const TString &rhs)
        {
            if (mData == rhs.mData)
                return true;
            if (mSize != rhs.mSize)
                return false;
            for (sizetype i = 0; i < mSize; ++i)
                if (CharFunc::toLower(mData[i]) != CharFunc::toLower(rhs.mData[i]))
                    return false;
            return true;
        }

        // 追加操作函数
        TString &append(Type ch)
        {
            if (mSize + 1 >= mCapc)
                realloc(mSize + 1);

            mData[mSize] = ch;
            mSize += 1;
            mData[mSize] = 0;
            return *this;
        }
        // 追加字符串中的前 n 个字符
        TString &append(const Type *const other, sizetype n = -1)
        {
            if (other != nullptr)
            {
                sizetype len = 0;
                const Type *p = other;
                while (*p++)
                    ++len;
                if (len > n)
                    len = n;
                if (len != 0)
                {
                    const auto size = mSize + len + 1;
                    if (size >= mCapc)
                        realloc(size);

                    memcpy(&mData[mSize], other, len);
                    mSize += len;
                    mData[mSize] = 0;
                }
            }
            return *this;
        }
        // 追加字符串中的前 n 个字符
        TString &append(const TString &other, sizetype n = -1)
        {
            if (other.empty() == false)
            {
                sizetype len = other.mSize;
                if (len > n)
                    len = n;
                if (len != 0)
                {
                    const auto size = mSize + len + 1;
                    if (size >= mCapc)
                        realloc(size);

                    memcpy(&mData[mSize], other.mData, len);
                    mSize += len;
                    mData[mSize] = 0;
                }
            }
            return *this;
        }

        // 确保有足够缓存
        inline void reserve(sizetype size)
        {
            ++size;
            if (size > mCapc)
                realloc(size);
            mData[mSize] = 0;
        }

        // 查找匹配函数

        /**
         * @brief 从起始位置 start开始前向查找字符串
         * @param str ：待查找的字符串
         * @param start ：查询的起始位置
         * @return 没有查到返回 -1，否则返回具体位置
         */
        sizetype find(const TString &str, sizetype start = 0) const
        {
            if (str.mData == mData)
                return 0;
            if (start > mSize || str.mSize == 0)
                return -1;

            for (sizetype i = start; i <= mSize - str.mSize; ++i)
            {
                sizetype j = 0;
                while (str.mData[j] && mData[i + j] == str.mData[j])
                    ++j;

                if (!str.mData[j])
                    return i;
            }

            return -1;
        }
        /**
         * @brief 从起始位置 start开始前向查找字符串
         * @param str ：待查找的字符串
         * @param start ：查询的起始位置
         * @return 没有查到返回 -1，否则返回具体位置
         */
        sizetype find(const Type *const str, sizetype start = 0) const
        {
            if (str == mData)
                return 0;
            if (str == nullptr || start > mSize)
                return -1;

            sizetype len = 0;

            while (str[len])
                ++len;

            for (sizetype i = start; i < mSize - len; ++i)
            {
                sizetype j = 0;
                while (str[j] && mData[i + j] == str[j])
                    ++j;

                if (!str[j])
                    return i;
            }

            return -1;
        }
        /**
         * @brief 从起始位置 start开始前向查找字符ch
         * @param ch :查找的字符
         * @param start :开始查找的位置
         * @return 没有查到返回 -1，否则返回具体位置
         */
        sizetype findFirst(Type ch, sizetype start = 0) const
        {
            for (sizetype i = start; i < mSize; ++i)
                if (mData[i] == ch)
                    return i;
            return -1;
        }
        /**
         * @brief 从起始位置 start开始逆向查找字符ch
         * @param ch :查找的字符
         * @param start :开始查找的位置
         * @return 没有查到返回 -1，否则返回具体位置
         */
        sizetype findLast(Type ch, sizetype start = -1) const
        {
            if (start >= mSize)
                start = mSize - 1;
            for (sizetype i = start; i >= 0; --i)
                if (mData[i] == ch)
                    return i;
            return -1;
        }
        /**
         * @brief 前向匹配存在字符列表的字符
         * @param list ：带匹配的字符列表
         * @param count ：字符列表长度
         *  @param start :开始查找的位置
         * @return 没有匹配返回 -1，否则返回具体位置
         */
        sizetype matchFirst(const Type *const list, sizetype count = 1, sizetype start = 0) const
        {
            if (list != nullptr && count != 0 && start <= mSize)
            {
                for (sizetype i = start; i < mSize; ++i)
                    for (sizetype j = 0; j < count; ++j)
                        if (mData[i] == list[j])
                            return i;
            }
            return -1;
        }
        inline sizetype matchFirst(const TString &list, sizetype start = 0) const { return matchFirst(list.mData, list.mSize, start); }
        /**
         * @brief 前向匹配不存在字符列表的字符
         * @param list ：带匹配的字符列表
         * @param count ：字符列表长度
         *  @param start :开始查找的位置
         * @return 没有匹配返回 -1，否则返回具体位置
         */
        sizetype matchFirstNot(const Type *const list, sizetype count = 1, sizetype start = 0) const
        {
            if (list != nullptr && count != 0 && start <= mSize)
            {
                for (sizetype i = start; i < mSize; ++i)
                {
                    sizetype j = 0;
                    for (; j < count; ++j)
                        if (mData[i] == list[j])
                            break;
                    if (j == count)
                        return i;
                }
            }
            return -1;
        }
        inline sizetype matchFirstNot(const TString &list, sizetype start = 0) const { return matchFirstNot(list.mData, list.mSize, start); }

        /**
         * @brief 逆向匹配存在字符列表的字符
         * @param list ：带匹配的字符列表
         * @param count ：字符列表长度
         *  @param start :开始查找的位置
         * @return 没有匹配返回 -1，否则返回具体位置
         */
        sizetype matchLast(const Type *const list, sizetype count = 1, sizetype start = -1) const
        {
            if (list != nullptr && count != 0)
            {
                if (start >= mSize)
                    start = mSize - 1;
                for (sizetype i = start; i >= 0; --i)
                    for (sizetype j = 0; j < count; ++j)
                        if (mData[i] == list[j])
                            return i;
            }
            return -1;
        }
        inline sizetype matchLast(const TString &list, sizetype start = -1) const { return matchLast(list.mData, list.mSize, start); }
        /**
         * @brief 逆向匹配不存在字符列表的字符
         * @param list ：带匹配的字符列表
         * @param count ：字符列表长度
         *  @param start :开始查找的位置
         * @return 没有匹配返回 -1，否则返回具体位置
         */
        sizetype matchLastNot(const Type *const list, sizetype count = 1, sizetype start = -1) const
        {
            if (list != nullptr && count != 0)
            {
                if (start >= mSize)
                    start = mSize - 1;
                for (sizetype i = start; i >= 0; --i)
                {
                    sizetype j = 0;
                    for (; j < count; ++j)
                        if (mData[i] == list[j])
                            break;
                    if (j == count)
                        return i;
                }
            }
            return -1;
        }
        inline sizetype matchLastNot(const TString &list, sizetype start = -1) const { return matchLastNot(list.mData, list.mSize, start); }
        /**
         * @brief 获取子字符串
         * @param start ：获取的起始位置
         * @param length ：获取字符串的长度
         * @return 获取的子字符串
         */
        TString substr(sizetype start, sizetype length) const
        {
            TString ret;
            if (length != 0 && start < mSize)
            {
                if (start + length > mSize)
                    length = mSize - start;
                // 确保长度
                ret.reserve(length);
                memcpy(ret.mData, &mData[start], length);
                /* for (sizetype i = 0; i < length; ++i)
                     ret.mData[i] = mData[i + start];*/
                ret.mSize = length;
                ret.mData[length] = 0;
            }
            return ret;
        }
        /**
         * @brief 替换字符
         * @param toReplace ：需要替换的字符
         * @param replaceWith ：用于替换的字符
         * @param start ：替换的起始位置
         * @return 替换后的字符串
         */
        TString &replace(Type toReplace, Type replaceWith, sizetype start = 0)
        {
            for (sizetype i = start; i < mSize; ++i)
                if (mData[i] == toReplace)
                    mData[i] = replaceWith;
            return *this;
        }
        /**
         * @brief 替换字符串
         * @param oldch ：需要替换的字符串
         * @param replaceWith ：用于替换的字符串
         * @param start ：替换的起始位置
         * @return 替换后的字符串
         */
        TString &replace(const TString &toReplace, const TString &replaceWith, sizetype start = 0)
        {
            if (toReplace.mSize == 0 || mSize == 0)
                return *this;

            const Type *other = toReplace.cstr();
            const Type *replace = replaceWith.cstr();
            const sizetype othersize = toReplace.mSize;
            const sizetype replacesize = replaceWith.mSize;

            int64 delta = replacesize - othersize;

            // 情况1：相当于字符替换，字符串长度不会变化
            if (delta == 0)
            {
                sizetype pos = start;
                while ((pos = find(toReplace, pos)) != -1)
                {
                    for (sizetype i = 0; i < replacesize; ++i)
                        mData[pos + i] = replace[i];
                    ++pos;
                }

                return *this;
            }
            // 情况2：字符串长度减少
            if (delta < 0)
            {
                sizetype i = start, find = 0;
                for (sizetype pos = start; pos < mSize; ++i, ++pos)
                {
                    // 尝试匹配字符串
                    if (mData[pos] == other[0])
                    {
                        sizetype j = 0;
                        for (; j < othersize; ++j)
                            if (mData[pos + j] != other[j])
                                break;
                        // 存在匹配，替换字符串
                        if (j == othersize)
                        {
                            ++find;
                            for (j = 0; j < replacesize; ++j)
                                mData[i + j] = replace[j];
                            i += replacesize - 1;
                            pos += othersize - 1;
                            continue;
                        }
                    }
                    // 不存在就复制字符
                    mData[i] = mData[pos];
                }
                mData[i] = 0;
                mSize = i;
                return *this;
            }

            // 情况3：字符串长度增加

            // 先确保内存足够
            sizetype findcount = 0;
            sizetype pos = start;
            while ((pos = find(toReplace, pos)) != -1)
            {
                ++findcount;
                ++pos;
            }
            // 分配内存
            sizetype len = delta * findcount + mSize + 1;
            if (len > mCapc)
                realloc(len);

            // 开始替换字符串
            pos = start;
            while ((pos = find(toReplace, pos)) != -1)
            {
                Type *start = mData + pos + othersize - 1;
                Type *ptr = mData + mSize;
                Type *end = mData + delta + mSize;

                // 移动字符以为替换字符串腾出空间。
                while (ptr != start)
                {
                    *end = *ptr;
                    --ptr;
                    --end;
                }

                // 添加字符
                for (sizetype i = 0; i < replacesize; ++i)
                    mData[pos + i] = replace[i];

                pos += replacesize;
                mSize += delta;
            }
            return *this;
        }

        /**
         * @brief 移除字符
         * @param ch ：需要移除的字符
         * @param start ：移除的起始位置
         * @return 移除后的字符串
         */
        TString &remove(const Type ch, sizetype start = 0)
        {
            sizetype pos = start, find = 0;
            for (sizetype i = start; i < mSize; ++i)
            {
                if (mData[i] == ch)
                {
                    ++find;
                    continue;
                }
                mData[pos++] = mData[i];
            }
            mSize -= find;
            mData[mSize] = 0;
            return *this;
        }
        /**
         * @brief 移除字符字符串
         * @param toRemove ：需要移除的字符串
         * @param start ：移除的起始位置
         * @return 移除后的字符串
         */
        TString &remove(const TString &toRemove, sizetype start = 0)
        {
            if (toRemove.mSize != 0)
            {
                sizetype pos = start, find = 0;
                for (sizetype i = start; i < mSize; ++i)
                {
                    sizetype j = 0;
                    while (j < toRemove.mSize)
                    {
                        if (mData[i + j] != toRemove.mData[j])
                            break;
                        ++j;
                    }
                    if (j == toRemove.mSize)
                    {
                        find += toRemove.mSize;
                        i += toRemove.mSize - 1;
                        continue;
                    }

                    mData[pos++] = mData[i];
                }
                mSize -= find;
                mData[mSize] = 0;
            }

            return *this;
        }
        /**
         * @brief 休整字符串，去掉头尾的休整字符
         * @param whitespace :需要休整的字符列表
         * @return 休整后的字符串
         */
        TString &trim(const TString &list = " \t\r\n")
        {
            sizetype begin = matchFirstNot(list);
            if (begin == -1)
                return *this;

            sizetype end = matchLastNot(list);
            if (end == -1)
                end = mSize;
            *this = substr(begin, end - begin);

            return *this;
        }
        /**
         * @brief 擦除字符
         * @param index ：擦除的位置索引
         * @return 擦除·后的字符串
         */
        TString &erase(sizetype index)
        {
            make_ensure(index < mSize);

            for (sizetype i = index; i <= mSize; ++i)
                mData[i - 1] = mData[i];
            --mSize;
            return *this;
        }
        /**
         * @brief 验证字符串是否有效
         * @return 有效字符串
         */
        TString &validate()
        {
            // 存在空字符
            for (sizetype i = 0; i < mCapc; ++i)
            {
                if (mData[i] == 0)
                {
                    mSize = i;
                    return *this;
                }
            }
            // 无结尾空字符
            if (mCapc > 0)
            {
                mSize = mCapc - 1;
                mData[mSize] = 0;
            }
            else
                clear();
            return *this;
        }

        // 操作符重载
        TString &operator=(const Type *rhs)
        {
            if (rhs == nullptr)
            {
                if (mData != nullptr)
                    mAlloc.dealloc(mData);
                mData = nullptr;
                mCapc = 0;
                mSize = 0;
            }
            else if ((uintptr)rhs != (uintptr)mData)
            {
                sizetype len = 0;
                const Type *p = (Type *)rhs;
                do
                {
                    ++len;
                } while (*p++);

                Type *old = mData;
                if (len > mCapc)
                {
                    mCapc = len;
                    mCapc = alignup<sizetype>(mCapc, 4);
                    mData = (Type *)mAlloc.allocArr(mCapc
#ifdef _check_memory_free
                                                    ,
                                                    this_file(), this_line()
#endif
                    );
                }
                memcpy(mData, rhs, len);
                mSize = len - 1;
                if (old != nullptr && old != mData)
                    mAlloc.dealloc(old);
            }
            return *this;
        }

        TString &operator=(const TString &rhs)
        {
            if (rhs.mData == nullptr)
            {
                if (mData != nullptr)
                    mAlloc.dealloc(mData);
                mData = nullptr;
                mCapc = 0;
                mSize = 0;
            }
            else if (rhs.mData != mData)
            {
                sizetype len = rhs.mSize + 1;

                Type *old = mData;
                if (len > mCapc)
                {
                    mCapc = len;
                    mCapc = alignup<sizetype>(mCapc, 4);
                    mData = (Type *)mAlloc.allocArr(mCapc
#ifdef _check_memory_free
                                                    ,
                                                    this_file(), this_line()
#endif
                    );
                }
                memcpy(mData, rhs.mData, len);
                mSize = len - 1;
                if (old != nullptr && old != mData)
                    mAlloc.dealloc(old);
            }
            return *this;
        }

        Type &operator[](sizetype index)
        {
            make_ensure(index < mSize);
            return mData[index];
        }

        bool operator==(const Type *const str) const
        {
            if (mData == str)
                return true;
            if (str == nullptr)
                return false;

            sizetype i = 0;
            for (; mData[i] && str[i]; ++i)
                if (mData[i] != str[i])
                    return false;

            return mData[i] == 0 && str[i] == 0;
        }
        bool operator==(const TString &rhs) const
        {
            if (mData == rhs.mData)
                return true;
            if (rhs.mData == nullptr || mSize != rhs.mSize)
                return false;

            sizetype i = 0;
            for (; mData[i] && rhs.mData[i]; ++i)
                if (mData[i] != rhs.mData[i])
                    return false;

            return mData[i] == 0 && rhs.mData[i] == 0;
        }

        bool operator!=(const Type *const str) const
        {
            if (mData == str)
                return false;
            if (str == nullptr)
                return true;

            sizetype i = 0;
            for (; mData[i] && str[i]; ++i)
                if (mData[i] != str[i])
                    return true;

            return !(mData[i] == 0 && str[i] == 0);
        }
        bool operator!=(const TString &rhs) const
        {
            if (mData == rhs.mData)
                return false;
            if (rhs.mData == nullptr || mSize != rhs.mSize)
                return true;

            sizetype i = 0;
            for (; mData[i] && rhs.mData[i]; ++i)
                if (mData[i] != rhs.mData[i])
                    return true;

            return !(mData[i] == 0 && rhs.mData[i] == 0);
        }

        // 追加整数
        TString &operator<<(int32 nVal)
        {
            Type bufer[72] = {0};
            Type *data = bufer;
            if (nVal == 0)
            {
                data[0] = '0';
                append_(data, 1);
                return *this;
            }
            // 不为 0
            bool neg = false;
            uint32 idx = 71;

            if (nVal < 0)
            {
                nVal = -nVal;
                neg = true;
            }
            while (nVal != 0 && idx != 0)
            {
                --idx;
                bufer[idx] = (Type)('0' + (nVal % 10));
                nVal /= 10;
            }
            if (neg == true)
            {
                --idx;
                bufer[idx] = '-';
            }
            append_(&bufer[idx], 71 - idx);
            return *this;
        }
        TString &operator<<(uint32 nVal)
        {
            Type bufer[72] = {0};
            Type *data = bufer;
            if (nVal == 0)
            {
                data[0] = '0';
                append_(data, 1);
                return *this;
            }
            // 不为 0
            uint32 idx = 71;

            while (nVal != 0 && idx != 0)
            {
                --idx;
                bufer[idx] = (Type)('0' + (nVal % 10));
                nVal /= 10;
            }
            append_(&bufer[idx], 71 - idx);
            return *this;
        }
        TString &operator<<(int64 nVal)
        {
            Type bufer[72] = {0};
            Type *data = bufer;
            if (nVal == 0)
            {
                data[0] = '0';
                append_(data, 1);
                return *this;
            }
            // 不为 0
            bool neg = false;
            uint32 idx = 71;

            if (nVal < 0)
            {
                nVal = -nVal;
                neg = true;
            }
            while (nVal != 0 && idx != 0)
            {
                --idx;
                bufer[idx] = (Type)('0' + (nVal % 10));
                nVal /= 10;
            }
            if (neg == true)
            {
                --idx;
                bufer[idx] = '-';
            }
            append_(&bufer[idx], 71 - idx);
            return *this;
        }
        TString &operator<<(uint64 nVal)
        {
            Type bufer[72] = {0};
            Type *data = bufer;
            if (nVal == 0)
            {
                data[0] = '0';
                append_(data, 1);
                return *this;
            }
            // 不为 0
            uint32 idx = 71;

            while (nVal != 0 && idx != 0)
            {
                --idx;
                bufer[idx] = (Type)('0' + (nVal % 10));
                nVal /= 10;
            }
            append_(&bufer[idx], 71 - idx);
            return *this;
        }
        TString &operator<<(flt32 nVal)
        {
            char tmpbuf[255];
            auto res = ::snprintf(tmpbuf, 255, "%0.6f", nVal);
            append_(tmpbuf, res);
            return *this;
        }
        TString &operator<<(flt64 nVal)
        {
            char tmpbuf[255];
            auto res = ::snprintf(tmpbuf, 255, "%0.8lf", nVal);
            append_(tmpbuf, res);
            return *this;
        }
        TString &operator<<(const Type *cstr)
        {
            append(cstr);
            return *this;
        }

    protected:
        // 重新分配内存
        void realloc(sizetype newsize)
        {
            Type *old = mData;
            // 大小4字节对齐
            newsize = alignup<sizetype>(newsize, 4);
            // 重新分配内存
            mData = (Type *)mAlloc.allocArr(newsize
#ifdef _check_memory_free
                                            ,
                                            this_file(), this_line()
#endif
            );

            mCapc = newsize;
            // 拷贝数据
            sizetype amount = mSize < newsize ? mSize : newsize;
            memcpy(mData, old, amount);

            if (mCapc < mSize)
                mSize = mCapc;
            // 释放内存
            if (old != nullptr)
                mAlloc.dealloc(old);
        }

        // 追加
        void append_(const Type *data, sizetype length)
        {
            if (data != nullptr && length != 0)
            {
                const auto size = mSize + length + 1;
                if (size >= mCapc)
                    realloc(size);

                memcpy(&mData[mSize], data, length);
                mSize += length;
                mData[mSize] = 0;
            }
        }

        sizetype mCapc;             // 缓存大小
        sizetype mSize;             // 真实大小
        Alloctor<Type, mem> mAlloc; // 内存分配器
        Type *mData;
    };

    // 多字节字符串
    using String = TString<char>;

    // 宽字符串
    using WString = TString<uint16>;

}
#endif //!__AIRSTRING__H__