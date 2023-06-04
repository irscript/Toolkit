#ifndef __AIRMATH3__H__
#define __AIRMATH3__H__
#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>
namespace air
{
    template <typename Type>
    struct Mat3
    {
    public:
        union
        {
            Type mArr[3 * 3];
            struct
            {
                Type m00, m01, m02;
                Type m10, m11, m12;
                Type m20, m21, m22;
            };
        };
        // 单位矩阵
        Mat3() : m00(1), m01(0), m02(0),
                 m10(0), m11(1), m12(0),
                 m20(0), m21(0), m22(1) {}

        Mat3(Type v)
            : m00(v), m01(0), m02(0),
              m10(0), m11(v), m12(0),
              m20(0), m21(0), m22(v) {}

        Mat3(const Mat3 &rhs)
            : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02),
              m10(rhs.m10), m11(rhs.m11), m12(rhs.m12),
              m20(rhs.m20), m21(rhs.m21), m22(rhs.m22) {}

        Mat3(Type v00, Type v01, Type v02,
             Type v10, Type v11, Type v12,
             Type v20, Type v21, Type v22)
            : m00(v00), m01(v01), m02(v02),
              m10(v10), m11(v11), m12(v12),
              m20(v20), m21(v21), m22(v22) {}

        Mat3(Type *v)
            : m00(v[0]), m01(v[1]), m02(v[2]),
              m10(v[3]), m11(v[4]), m12(v[5]),
              m20(v[6]), m21(v[7]), m22(v[8]) {}

        bool equals(const Mat3 &rhs) const
        {
            if (Math::equals(m00, rhs.m00) == false)
                return false;
            if (Math::equals(m01, rhs.m01) == false)
                return false;
            if (Math::equals(m02, rhs.m02) == false)
                return false;

            if (Math::equals(m10, rhs.m10) == false)
                return false;
            if (Math::equals(m11, rhs.m11) == false)
                return false;
            if (Math::equals(m12, rhs.m12) == false)
                return false;

            if (Math::equals(m20, rhs.m20) == false)
                return false;
            if (Math::equals(m21, rhs.m21) == false)
                return false;
            if (Math::equals(m22, rhs.m22) == false)
                return false;
            return true;
        }

        // 重载操作数
        Mat3 &operator=(const Mat3 &rhs)
        {
            m00 = rhs.m00, m01 = rhs.m01, m02 = rhs.m02;
            m10 = rhs.m10, m11 = rhs.m11, m12 = rhs.m12;
            m20 = rhs.m20, m21 = rhs.m21, m22 = rhs.m22;
            return *this;
        }
        bool operator==(const Mat3 &rhs) const
        {
            if (m00 != rhs.m00)
                return false;
            if (m01 != rhs.m01)
                return false;
            if (m02 != rhs.m02)
                return false;

            if (m10 != rhs.m10)
                return false;
            if (m11 != rhs.m11)
                return false;
            if (m12 != rhs.m12)
                return false;

            if (m20 != rhs.m20)
                return false;
            if (m21 != rhs.m21)
                return false;
            if (m22 != rhs.m22)
                return false;

            return true;
        }
        bool operator!=(const Mat3 &rhs) const { return !(*this == rhs); }

        Mat3 operator+(const Mat3 &rhs) const
        {
            return Mat3(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                        m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                        m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22);
        }
        Mat3 operator-(const Mat3 &rhs) const
        {
            return Mat3(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                        m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                        m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22);
        }

        Mat3 &operator+=(const Mat3 &rhs)
        {
            m00 += rhs.m00, m01 += rhs.m01, m02 += rhs.m02;
            m10 += rhs.m10, m11 += rhs.m11, m12 += rhs.m12;
            m20 += rhs.m20, m21 += rhs.m21, m22 += rhs.m22;
            return *this;
        }
        Mat3 &operator-=(const Mat3 &rhs)
        {
            m00 -= rhs.m00, m01 -= rhs.m01, m02 -= rhs.m02;
            m10 -= rhs.m10, m11 -= rhs.m11, m12 -= rhs.m12;
            m20 -= rhs.m20, m21 -= rhs.m21, m22 -= rhs.m22;
            return *this;
        }

        Mat3 operator*(Type rhs) const
        {
            return Mat3(m00 * rhs, m01 * rhs, m02 * rhs,
                        m10 * rhs, m11 * rhs, m12 * rhs,
                        m20 * rhs, m21 * rhs, m22 * rhs);
        }
        Mat3 operator*=(Type rhs)
        {
            m00 *= rhs, m01 *= rhs, m02 *= rhs;
            m10 *= rhs, m11 *= rhs, m12 *= rhs;
            m20 *= rhs, m21 *= rhs, m22 *= rhs;
            return *this;
        }
    };
}
#endif //!__AIRMATH3__H__