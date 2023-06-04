#ifndef __AIRRECT__H__
#define __AIRRECT__H__

#include <airkit/Math/airVec2.h>

namespace air
{
    // 矩形定义
    template <typename Type>
    struct Rect
    {
        Vec2<Type> mLT, mRB; // 左上右下
        Rect() : mLT(), mRB() {}
        Rect(const Vec2<Type> &rt, const Vec2<Type> &lb) : mLT(rt), mRB(lb) {}

        Rect(Type left, Type top, Type right, Type bottom)
            : mLT(left, top), mRB(right, bottom) {}

        explicit Rect(const Type *data) : mLT(data[0], data[1]),
                                          mRB(data[2], data[3]) {}

        String toString() const
        {
            String ret("Rect[ L: ");
            ret << mLT.mX << "\tT: " << mLT.mY << "\tR: " << mRB.mX << "\tB: " << mRB.mY << " ]";
            ret << "[ W: " << width() << "\tH: " << height() << "]";
            return ret;
        }

        void define(const Rect &rect)
        {
            mLT = rect.mLT;
            mRB = rect.mRB;
        }
        void define(const Vec2<Type> &rt, const Vec2<Type> &lb)
        {
            mLT = rt;
            mRB = lb;
        }
        void define(Type left, Type top, Type right, Type bottom)
        {
            mLT.set(left, top);
            mRB.set(right, bottom);
        }
        // 合并
        void marge(const Vec2<Type> &point)
        {
            // 左上
            if (point.mX < mLT.mX)
                mLT.mX = point.mX;
            if (point.mY > mLT.mY)
                mLT.mY = point.mY;
            // 右下
            if (point.mX > mRB.mX)
                mRB.mX = point.mX;
            if (point.mY < mRB.mY)
                mRB.mY = point.mY;
        }
        void marge(const Rect &rect)
        {
            // 左上
            if (rect.mLT.mX < mLT.mX)
                mLT.mX = rect.mLT.mX;
            if (rect.mLT.mY > mLT.mY)
                mLT.mY = rect.mLT.mY;
            // 右下
            if (rect.mLT.mX > mRB.mX)
                mRB.mX = rect.mLT.mX;
            if (rect.mLT.mY < mRB.mY)
                mRB.mY = rect.mLT.mY;
        }

        // 清空
        void clear()
        {
            mLT.set(0, 0);
            mRB.set(0, 0);
        }

        // 获取中心
        Vec2<Type> center() const
        {
            Type cx = (mLT.mX + mRB.mX) * 0.5;
            Type cy = (mLT.mY + mRB.mY) * 0.5;
            return Vec2<Type>(cx, cy);
        }

        // 获取宽度
        Type width() const { return mRB.mX - mLT.mX; }
        // 获取高度
        Type height() const { return mLT.mY - mRB.mY; }

        // 是否相同，误差内
        bool equals(const Rect &rect) const { return mLT.equals(rect.mLT) && mRB.equals(rect.mRB); }
        // 点是否在矩形内部(含边线)
        bool inside(const Vec2<Type> &point) const { return point.mX < mLT.mX || point.mX > mRB.mX || point.mY > mLT.mY || point.mY < mRB.mY; }

        // 裁剪矩阵（获得交集）
        Rect clip(const Rect &rect) const
        {
            Type left, top, right, bottom;
            // 找最小
            left = rect.mLT.mX < mLT.mX ? mLT.mX : rect.mLT.mX;
            top = rect.mLT.mY < mLT.mY ? rect.mLT.mY : mLT.mY;
            // 找最大
            right = rect.mRB.mX > mRB.mX ? mRB.mX : rect.mRB.mX;
            bottom = rect.mRB.mY > mRB.mY ? rect.mRB.mY : mRB.mY;

            return Rect(left, top, right, bottom);
        }

        // 操作符重载

        Rect &operator=(const Rect &rhs)
        {
            mLT = rhs.mLT;
            mRB = rhs.mRB;
            return *this;
        }

        bool operator==(const Rect &rhs) const { return mLT == rhs.mLT && mRB == rhs.mRB; }
        bool operator!=(const Rect &rhs) const { return mLT != rhs.mLT || mRB != rhs.mRB; }
    };

    using Recti = Rect<int32>;
    using Rectf = Rect<flt32>;
    using Rectd = Rect<flt64>;
    using Rectr = Rect<real>;
}
#endif //!__AIRRECT__H__