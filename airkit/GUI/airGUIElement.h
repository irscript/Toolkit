#ifndef __AIRGUIELEMENT__H__
#define __AIRGUIELEMENT__H__

#include <airkit/Core/airObject.h>
#include <airkit/Struct/airIList.h>

// GUI 控件基类
namespace air
{
    // 所以UI控件的基类
    struct IGUIElement : public IListNode<IGUIElement>
    {
        IGUIElement() {}
        virtual ~IGUIElement() {}
        /* data */
    };

}
#endif //!__AIRGUIELEMENT__H__