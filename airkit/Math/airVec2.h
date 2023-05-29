#ifndef __AIRVEC2__H__
#define __AIRVEC2__H__

#include <airkit/Core/airTypes.h>
namespace air
{
    template <typename Type>
    struct Vec2
    {
        
    public:
        union
        {
            Type mArr[2];
            Type mX, mY;
        };
    };

}
#endif //!__AIRVEC2__H__