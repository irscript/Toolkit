#ifndef __AIRGUIELEMENT__H__
#define __AIRGUIELEMENT__H__

#include <airkit/Core/airObject.h>
#include <airkit/Struct/airIList.h>
#include <airkit/Math/airMath.h>

#include <airkit/Render/UI/airUIDrawData.h>
#include <airkit/Render/Event/airEvent.h>
#include <airkit/Render/UI/airUIFlag.h>

// GUI 控件基类
namespace air
{
    /** 控件坐标系
     * (0,0)
     *  (L,T)----------------------------> X
     *      |                       |
     *      |                       |
     *      |                       |
     *      |-----------------------|(R,B)
     *      V
     *      Y
     * width=B-T
     * height=R-L
     */

    // 窗口基类
    struct IWindow;

    // UI控件基本类：窗口和控件共有
    struct IGUIElement : public IRefObject, IListNode<IGUIElement>
    {
        IGUIElement() : mUIFlag(0), mParent(nullptr), mEventFilter(nullptr) {}
        ~IGUIElement() {}

        //------------------基本工具函数-----------------
        // 设置事件过滤器
        inline void setEventListener(IEventListener *filter) { mEventFilter = filter; }
        // 设置父类控件
        inline void setParent(IGUIElement *parent);
        // 移除子控件
        inline void removeChild(IGUIElement *child);
        // 添加子控件
        inline void addChild(IGUIElement *child);
        // 子控件置顶
        inline void topChild(IGUIElement *child);

        // 改变位置
        inline void setPos(Vec2f &pos);
        // 改变尺寸
        inline void setSize(Vec2f &pos);

        // 移动控件
        inline void moveTo(const Rectf &rect);

        // -------------------坐标转换----------------
        // 从屏幕坐标转窗口坐标
        inline void screenToWindow(Vec2f &pos) const;
        // 从窗口坐标转屏幕坐标
        inline void windowToScreen(Vec2f &pos) const;

        // 从窗口坐标转到客户坐标
        inline void windowToClient(Vec2f &pos) const;
        // 从客户坐标转到窗口坐标
        inline void clientToWindow(Vec2f &pos) const;

        // 从屏幕坐标转客户坐标
        inline void screenToClient(Vec2f &pos) const;
        // 从客户坐标转屏幕坐标
        inline void clientToScreen(Vec2f &pos) const;

        // 获取窗口
        inline IWindow *getWindow() const;

        // -------------------事件响应----------------
        // 渲染总响应事件
        virtual void onEvent(const IEvent &event) {}

        // 响应大小改变事件
        virtual void onSize(flt32 width, flt32 height) {}
        // 响应鼠标按下事件
        virtual void onMousePress(MouseButton button) {}
        // 响应鼠标释放事件
        virtual void onMouseRelease(MouseButton button) {}

        // --------------------UI绘制相关-----------------
        /** @brief 生成渲染顶点数据
         * @param step :渲染帧时间步，主要用于动画渲染
         * @param drawlist :生成的渲染列表
         * @param clipRect :待渲染控件的裁剪矩形
         */
        virtual void onRender(const FrameEvent &step, UIDrawList &drawlist, const Rectf &clipRect) {}

        //------------------UI标记------------------

        // 当前控件是不是窗口
        inline bool isWindow() const { return mUIFlag & UIFlag::Window; }

        // 是否获得焦点
        inline bool isFocus() const { return mUIFlag & UIFlag::Focus; }
        // 设置焦点标志
        inline void setFocus(bool flag) { flag ? setUIFlag(UIFlag::Focus) : unsetUIFlag(UIFlag::Focus); }

        // 是否可见
        inline bool isVisible() const { return mUIFlag & UIFlag::Visible; }
        // 设置可见标志
        inline void setVisible(bool flag) { flag ? setUIFlag(UIFlag::Visible) : unsetUIFlag(UIFlag::Visible); }

        // 是否可用
        inline bool isEnable() const { return mUIFlag & UIFlag::Enable; }
        // 设置可用标志
        inline void setEnable(bool flag) { flag ? setUIFlag(UIFlag::Enable) : unsetUIFlag(UIFlag::Enable); }

        // 是否可绘制
        inline bool isDrawable() const { return mUIFlag & UIFlag::Drawable; }
        // 设置可绘制标志
        inline void setDrawable(bool flag) { flag ? setUIFlag(UIFlag::Drawable) : unsetUIFlag(UIFlag::Drawable); }

    protected:
        // 将某个标识位置 1
        inline void setUIFlag(UIFlag flag) { mUIFlag |= flag; }
        // 将某个标识位置 0
        inline void unsetUIFlag(UIFlag flag) { mUIFlag &= (~flag); }

        IList<IGUIElement> mChildUI;  // 子控件元素
        IGUIElement *mParent;         // 该UI的父类控件：为window时，值为nullptr
        IEventListener *mEventFilter; // 事件监听过滤器

        Rectf mArea;    // 控件在父类的矩形区域：裁剪窗口
        RGBA32 mBkclr;  // 控件的背景颜色
        uint32 mUIFlag; // UI标记
    };
    // 窗口接口
    struct IWindow : public IGUIElement
    {
        IWindow()
            : IGUIElement(), mNative(nullptr) { mUIFlag = (uint32)UIFlag::Window; }
        // 获取帧渲染时间事件
        inline FrameEvent &getFrameEvent() { return mTimeStep; }

    protected:
        FrameEvent mTimeStep; // 帧渲染时间步
        uintptr mNative;      // 本地窗口指针
    };
    // 包含内联文件

#include <airkit/Render/UI/airGUIElement.inl>

}
#endif //!__AIRGUIELEMENT__H__