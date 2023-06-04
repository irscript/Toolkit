#ifndef __AIRVEC2__H__
#define __AIRVEC2__H__

#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>

namespace air
{
    template <typename Type>
    struct Vec2
    {

    public:
        union
        {
            Type mArr[2];
            struct
            {
                Type mX, mY;
            };
        };

        Vec2() : mX(0), mY(0) {}
        Vec2(Type v) : mX(v), mY(v) {}
        Vec2(Type x, Type y) : mX(x), mY(y) {}
        Vec2(Type v[]) : mX(v[0]), mY(v[1]) {}
        Vec2(const Vec2 &v) : mX(v.mX), mY(v.mY) {}

        Type x() const { return mX; }
        Type y() const { return mY; }

        String toString() const
        {
            String ret("Vex2[ mX: ");
            ret << mX << "\tmY: " << mY << " ]";
            return ret;
        }
        // 带误差判断是否等于
        bool equals(const Vec2 &rhs) const { return Math::equals(mX, rhs.mX) && Math::equals(mY, rhs.mY); }

        void set(Type v) { mX = v, mY = v; }
        void set(Type v[]) { mX = v[0], mY = v[1]; }
        void set(Type x, Type y) { mX = x, mY = y; }

        Type *data() { return mArr; }

        // 获取平方根长度
        Type length() const { return Math::sqrt(mX * mX + mY * mY); }
        // 获取平方长度
        Type length2() const { return mX * mX + mY * mY; }

        // 点积
        Type dot(const Vec2 &rhs) const { return mX * rhs.mX + mY * rhs.mY; }
        // 两点之间的距离
        Type distance(const Vec2 &rhs) const { return Math::sqrt((mX - rhs.mX) * (mX - rhs.mX) + (mY - rhs.mY) * (mY - rhs.mY)); }
        // 两点之间的平方距离
        Type distance2(const Vec2 &rhs) const { return (mX - rhs.mX) * (mX - rhs.mX) + (mY - rhs.mY) * (mY - rhs.mY); }
        // 归一化
        Vec2 &normalize()
        {
            flt64 len = (flt64)(mX * mX + mY * mY);
            if (len != 0)
            {
                len = 1.0 / Math::sqrt(len);
                mX = (Type)(mX * len);
                mY = (Type)(mY * len);
            }
            return *this;
        }
        // 归一化的向量
        Vec2 normalized()
        {
            Vec2 ret = *this;
            return ret.normalize();
        }

        // =============重载操作符====================
        bool operator==(const Vec2 &rhs) const { return mX == rhs.mX && mY == rhs.mY; }
        bool operator!=(const Vec2 &rhs) const { return mX != rhs.mX || mY != rhs.mY; }

        Vec2 operator-() const { return Vec2(-mX, -mY); }

        Vec2 &operator=(const Vec2 &rhs)
        {
            mX = rhs.mX, mY = rhs.mY;
            return *this;
        }

        Vec2 operator+(const Vec2 &rhs) const { return Vec2(rhs.mX + mX, rhs.mY + mY); }
        Vec2 &operator+=(const Vec2 &rhs)
        {
            mX += rhs.mX, mY += rhs.mY;
            return *this;
        }
        Vec2 operator-(const Vec2 &rhs) const { return Vec2(rhs.mX - mX, rhs.mY - mY); }
        Vec2 &operator-=(const Vec2 &rhs)
        {
            mX -= rhs.mX, mY -= rhs.mY;
            return *this;
        }

        Vec2 operator*(const Vec2 &rhs) const { return Vec2(rhs.mX * mX, rhs.mY * mY); }
        Vec2 &operator*=(const Vec2 &rhs)
        {
            mX *= rhs.mX, mY *= rhs.mY;
            return *this;
        }
        Vec2 operator/(const Vec2 &rhs) const { return Vec2(mX / rhs.mX, mY / rhs.mY); }
        Vec2 &operator/=(const Vec2 &rhs)
        {
            mX /= rhs.mX, mY /= rhs.mY;
            return *this;
        }

        Vec2 operator+(const Type rhs) const { return Vec2(rhs + mX, rhs + mY); }
        Vec2 &operator+=(const Type rhs)
        {
            mX += rhs, mY += rhs;
            return *this;
        }
        Vec2 operator-(const Type rhs) const { return Vec2(mX - rhs, mY - rhs); }
        Vec2 &operator-=(const Type rhs)
        {
            mX -= rhs, mY -= rhs;
            return *this;
        }

        Vec2 operator*(const Type rhs) const { return Vec2(rhs * mX, rhs * mY); }
        Vec2 &operator*=(const Type rhs)
        {
            mX *= rhs, mY *= rhs;
            return *this;
        }
        Vec2 operator/(const Type rhs) const { return Vec2(mX / rhs, mY / rhs); }
        Vec2 &operator/=(const Type rhs)
        {
            mX /= rhs, mY /= rhs;
            return *this;
        }
    };
    // 预定义
    using Vec2i = Vec2<int32>;
    using Vec2u = Vec2<uint32>;
    using Vec2f = Vec2<flt32>;
    using Vec2d = Vec2<flt64>;
    using Vec2r = Vec2<real>;
}
#endif //!__AIRVEC2__H__