#ifndef __AIRAPP__H__
#define __AIRAPP__H__
#include <airkit/Core/airTypes.h>
namespace air
{

    class Application
    {
    public:
        Application();
        virtual ~Application();

        virtual int32 run() = 0;
    };
}

#endif //!__AIRAPP__H__