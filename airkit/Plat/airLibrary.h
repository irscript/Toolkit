#ifndef __AIRLIBRARY__H__
#define __AIRLIBRARY__H__
#include <airkit/Core/airObject.h>
namespace air
{
    class Library : public IRefObject
    {
    private:
        uintptr mHandle; // 动态句柄

    public:
        Library() : mHandle(nullptr) {}
        virtual ~Library()
        {
            if (mHandle != nullptr)
                close();
        }

        Bool open(cstring path);
        void close();
        uintptr symbols(cstring name);
    };
}
#endif //!__AIRLIBRARY__H__