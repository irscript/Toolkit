#ifndef __AIRVEC4__H__
#define __AIRVEC4__H__
#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>
#include <airkit/Math/airVec3.h>

namespace air
{
    template <typename Type>
    struct Vec4
    {
    public:
        union
        {
            Type mArr[4];
            struct
            {
                Type mX, mY, mZ, mW;
            };
        };

        static Vec4 cZero;
        static Vec4 cOne;

    public:
        Vec4() : mX(0), mY(0), mZ(0), mW(0) {}
        Vec4(Type nVal)
            : mX(nVal), mY(nVal), mZ(nVal), mW(nVal) {}
        Vec4(const Vec4 &rhs)
            : mX(rhs.mX), mY(rhs.mY), mZ(rhs.mZ), mW(rhs.mW) {}
        Vec4(const Vec3<Type> &v3, Type w)
            : mX(v3.mX), mY(v3.mY), mZ(v3.mZ), mW(w) {}
        Vec4(Type x, Type y, Type z, Type w)
            : mX(x), mY(y), mZ(z), mW(w) {}
        Vec4(Type *arr) : mX(arr[0]), mY(arr[1]), mZ(arr[2]), mW(arr[3]) {}

        Type x() const { return mX; }
        Type y() const { return mY; }
        Type z() const { return mZ; }
        Type w() const { return mW; }

        void set(Type v) { mX = v, mY = v, mZ = v, mW = v; }
        void set(Type v[]) { mX = v[0], mY = v[1], mZ = v[2], mW = v[3]; }
        void set(Type x, Type y, Type z, Type w = 1) { mX = x, mY = y, mZ = z, mW = w; }

        String toString() const
        {
            String ret("Vex4[ mX: ");
            ret << mX << "\tmY: " << mY << "\tmZ: " << mZ << "\tmW: " << mW << " ]";
            return ret;
        }

        // 点积
        Type dot(const Vec4 &rhs) const { return mX * rhs.mX + mY * rhs.mY + mZ * rhs.mY + mW * rhs.mW; }
        // 插值
        Vec4 lerp(const Vec4 &rhs, real t) const { return Vec4(*this * (1.0f - t) + rhs * t); }

        // 判断是否相等，对于浮点是带误差判断的
        bool equals(const Vec4 &rhs) const { return Math::equals(mX, rhs.mX) && Math::equals(mY, rhs.mY) && Math::equals(mZ, rhs.mZ) && Math::equals(mW, rhs.mW); }

        Type *data() { return mArr; }

        // 重载操作符

        Vec4 &operator=(const Vec4 &rhs)
        {
            mX = rhs.mX, mY = rhs.mY, mZ = rhs.mZ, mW = rhs.mW;
            return *this;
        }
        bool operator==(const Vec4 &rhs) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW; }
        bool operator!=(const Vec4 &rhs) const { return mX != rhs.mX || mY != rhs.mY || mZ != rhs.mZ || mW != rhs.mW; }

        Vec4 operator+(const Vec4 &rhs) const { return Vec4(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW); }
        Vec4 operator-(const Vec4 &rhs) const { return Vec4(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW); }
        Vec4 operator*(const Vec4 &rhs) const { return Vec4(mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ, mW * rhs.mW); }
        Vec4 operator/(const Vec4 &rhs) const { return Vec4(mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ, mW / rhs.mW); }

        Vec4 operator+(Type rhs) const { return Vec4(mX + rhs, mY + rhs, mZ + rhs, mW + rhs); }
        Vec4 operator-(Type rhs) const { return Vec4(mX - rhs, mY - rhs, mZ - rhs, mW - rhs); }
        Vec4 operator*(Type rhs) const { return Vec4(mX * rhs, mY * rhs, mZ * rhs, mW * rhs); }
        Vec4 operator/(Type rhs) const { return Vec4(mX / rhs, mY / rhs, mZ / rhs, mW / rhs); }

        Vec4 &operator+=(const Vec4 &rhs)
        {
            mX += rhs.mX, mY += rhs.mY, mZ += rhs.mZ, mW += rhs.mW;
            return *this;
        }
        Vec4 &operator-=(const Vec4 &rhs)
        {
            mX -= rhs.mX, mY -= rhs.mY, mZ -= rhs.mZ, mW -= rhs.mW;
            return *this;
        }
        Vec4 &operator*=(const Vec4 &rhs)
        {
            mX *= rhs.mX, mY *= rhs.mY, mZ *= rhs.mZ, mW *= rhs.mW;
            return *this;
        }
        Vec4 &operator/=(const Vec4 &rhs)
        {
            mX /= rhs.mX, mY /= rhs.mY, mZ /= rhs.mZ, mW /= rhs.mW;
            return *this;
        }

        Vec4 &operator+=(Type rhs)
        {
            mX += rhs, mY += rhs, mZ += rhs, mW += rhs;
            return *this;
        }
        Vec4 &operator-=(Type rhs)
        {
            mX -= rhs, mY -= rhs, mZ -= rhs, mW -= rhs;
            return *this;
        }
        Vec4 &operator*=(Type rhs)
        {
            mX *= rhs, mY *= rhs, mZ *= rhs, mW *= rhs;
            return *this;
        }
        Vec4 &operator/=(Type rhs)
        {
            mX /= rhs, mY /= rhs, mZ /= rhs, mW /= rhs;
            return *this;
        }
    };

    using Vec4i = Vec4<int32>;
    using Vec4u = Vec4<uint32>;
    using Vec4f = Vec4<flt32>;
    using Vec4d = Vec4<flt64>;
    using Vec4r = Vec4<real>;
}
#endif //!__AIRVEC4__H__