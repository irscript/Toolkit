#ifndef __AIRVEC3__H__
#define __AIRVEC3__H__

#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>
namespace air
{
    template <typename Type>
    struct Vec3
    {
    public:
        union
        {
            Type mArr[3];
            struct
            {
                Type mX, mY, mZ;
            };
        };

        static Vec3 cZero;
        static Vec3 cOne;
        static Vec3 cLeft;
        static Vec3 cRight;
        static Vec3 cUp;
        static Vec3 cDown;
        static Vec3 cForward;
        static Vec3 cBack;

    public:
        Vec3() : mX(0), mY(0), mZ(0) {}
        Vec3(Type nVal) : mX(nVal), mY(nVal), mZ(nVal) {}
        Vec3(Type x, Type y, Type z) : mX(x), mY(y), mZ(z) {}
        Vec3(Type *arr) : mX(arr[0]), mY(arr[1]), mZ(arr[2]) {}
        Vec3(const Vec3 &rhs) : mX(rhs.mX), mY(rhs.mY), mZ(rhs.mZ) {}

        Type x() const { return mX; }
        Type y() const { return mY; }
        Type z() const { return mZ; }

        void set(Type v) { mX = v, mY = v, mZ = v; }
        void set(Type v[]) { mX = v[0], mY = v[1], mZ = v[2]; }
        void set(Type x, Type y, Type z) { mX = x, mY = y, mZ = z; }

        String toString() const
        {
            String ret("Vex3[ mX: ");
            ret << mX << "\tmY: " << mY << "\tmZ: " << mZ << " ]";
            return ret;
        }
        // 归一化
        Vec3 &normalize()
        {
            return *this;
        }
        // 点积
        Type dot(const Vec3 &rhs) const { return mX * rhs.mX + mY * rhs.mY + mZ * rhs.mZ; }
        // 内积
        Vec3 cross(const Vec3 &rhs) const
        {
            return Vec3(mY * rhs.mZ - mZ * rhs.mY,
                        mZ * rhs.mX - mX * rhs.mZ,
                        mX * rhs.mY - mY * rhs.mX);
        }
        // 长度
        Type length() const { return Math::sqrt(mX * mX + mY * mY + mZ * mZ); }
        // 平方长度
        Type length2() const { return mX * mX + mY * mY + mZ * mZ; }

        Type *data() { return mArr; }
        // 取顶值
        void getCeil(Vec3 cmp)
        {
            if (cmp.mX > mX)
                mX = cmp.mX;
            if (cmp.mY > mY)
                mY = cmp.mY;
            if (cmp.mZ > mZ)
                mZ = cmp.mZ;
        }
        // 取底值
        void getFloor(Vec3 cmp)
        {
            if (cmp.mX < mX)
                mX = cmp.mX;
            if (cmp.mY < mY)
                mY = cmp.mY;
            if (cmp.mZ < mZ)
                mZ = cmp.mZ;
        }
        // 绕X轴旋转
        Vec3 rotaeX(real angle) const
        {
            flt64 const rad = Math::deg2rad(angle);
            flt64 const cos = Math::cos(rad);
            flt64 const sin = Math::sin(rad);
            auto y = mY * cos - mZ * sin;
            auto z = mY * sin + mZ * cos;
            return Vec3(mX, y, z);
        }
        // 绕Y轴旋转
        Vec3 rotaeY(real angle) const
        {
            flt64 const rad = Math::deg2rad(angle);
            flt64 const cos = Math::cos(rad);
            flt64 const sin = Math::sin(rad);
            auto x = mX * cos + mZ * sin;
            auto z = -mX * sin + mZ * cos;
            return Vec3(x, mY, z);
        }
        // 绕Z轴旋转
        Vec3 rotaeZ(real angle) const
        {
            flt64 const rad = Math::deg2rad(angle);
            flt64 const cos = Math::cos(rad);
            flt64 const sin = Math::sin(rad);
            auto x = mX * cos - mY * sin;
            auto y = mX * sin + mY * cos;
            return Vec3(x, y, mZ);
        }
        // 插值
        Vec3 lerp(const Vec3 &rhs, real t) const { return *this * (1.0f - t) + rhs * t; }

        // 重载操作符
        Vec3 &operator=(const Vec3 &rhs)
        {
            mX = rhs.mX;
            mY = rhs.mY;
            mZ = rhs.mZ;
            return *this;
        }

        bool operator==(const Vec3 &rhs) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ; }
        bool operator!=(const Vec3 &rhs) const { return mX != rhs.mX || mY != rhs.mY || mZ != rhs.mZ; }

        Vec3 operator+(const Vec3 &rhs) const { return Vec3(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ); }
        Vec3 operator-(const Vec3 &rhs) const { return Vec3(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ); }
        Vec3 operator*(const Vec3 &rhs) const { return Vec3(mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ); }
        Vec3 operator/(const Vec3 &rhs) const { return Vec3(mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ); }

        Vec3 operator+(const Type &rhs) const { return Vec3(mX + rhs, mY + rhs, mZ + rhs); }
        Vec3 operator-(const Type &rhs) const { return Vec3(mX - rhs, mY - rhs, mZ - rhs); }
        Vec3 operator*(const Type &rhs) const { return Vec3(mX * rhs, mY * rhs, mZ * rhs); }
        Vec3 operator/(const Type &rhs) const { return Vec3(mX / rhs, mY / rhs, mZ / rhs); }

        Vec3 &operator+=(const Vec3 &rhs)
        {
            mX += rhs.mX, mY += rhs.mY, mZ += rhs.mZ;
            return *this;
        }
        Vec3 &operator-=(const Vec3 &rhs)
        {
            mX -= rhs.mX, mY -= rhs.mY, mZ -= rhs.mZ;
            return *this;
        }
        Vec3 &operator*=(const Vec3 &rhs)
        {
            mX *= rhs.mX, mY *= rhs.mY, mZ *= rhs.mZ;
            return *this;
        }
        Vec3 &operator/=(const Vec3 &rhs)
        {
            mX /= rhs.mX, mY /= rhs.mY, mZ /= rhs.mZ;
            return *this;
        }

        Vec3 &operator+=(const Type &rhs)
        {
            mX += rhs, mY += rhs, mZ += rhs;
            return *this;
        }
        Vec3 &operator-=(const Type &rhs)
        {
            mX -= rhs, mY -= rhs, mZ -= rhs;
            return *this;
        }
        Vec3 &operator*=(const Type &rhs)
        {
            mX *= rhs, mY *= rhs, mZ *= rhs;
            return *this;
        }
        Vec3 &operator/=(const Type &rhs)
        {
            mX /= rhs, mY /= rhs, mZ /= rhs;
            return *this;
        }
    };

    using Vec3i = Vec3<int32>;
    using Vec3u = Vec3<uint32>;
    using Vec3f = Vec3<flt32>;
    using Vec3d = Vec3<flt64>;
    using Vec3r = Vec3<real>;
}
#endif //!__AIRVEC3__H__