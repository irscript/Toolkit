#ifndef __AIRCOLOR__H__
#define __AIRCOLOR__H__
#include <airkit/Core/airTypes.h>

namespace air
{
    // 颜色
    struct RGBA32
    {
        union
        {
            uint8 arr[4];
            struct
            {
                uint8 r, g, b, a;
            };
            uint32 rgba;
        };
    };
}

#endif //!__AIRCOLOR__H__