#ifndef __AIRMAT2__H__
#define __AIRMAT2__H__
#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>
namespace air
{
    template <typename Type>
    struct Mat2
    {
    public:
        union
        {
            Type mArr[2 * 2];
            struct
            {
                Type m00, m01;
                Type m10, m11;
            };
        };

    public:
        // 单位矩阵
        Mat2() : m00(1), m01(0),
                 m10(0), m11(1) {}

        Mat2(const Mat2 &rhs) : m00(rhs.m00), m01(rhs.m01),
                                m10(rhs.m10), m11(rhs.m11) {}

        Mat2(Type v00, Type v01, Type v10, Type v11) : m00(v00), m01(v01),
                                                       m10(v10), m11(v11) {}

        Mat2(Type *v) : m00(v[0]), m01(v[1]),
                        m10(v[2]), m11(v[3]) {}

        bool equals(const Mat2 &rhs) const
        {
            if (Math::equals(m00, rhs.m00) == false)
                return false;
            if (Math::equals(m01, rhs.m01) == false)
                return false;

            if (Math::equals(m10, rhs.m10) == false)
                return false;
            if (Math::equals(m11, rhs.m11) == false)
                return false;

            return true;
        }

        // 重载操作数
        Mat2 &operator=(const Mat2 &rhs)
        {
            m00 = rhs.m00, m01 = rhs.m01,
            m10 = rhs.m10, m11 = rhs.m11;
            return *this;
        }
        bool operator==(const Mat2 &rhs) const
        {
            if (m00 != rhs.m00)
                return false;
            if (m01 != rhs.m01)
                return false;

            if (m10 != rhs.m10)
                return false;
            if (m11 != rhs.m11)
                return false;
            return true;
        }
        bool operator!=(const Mat2 &rhs) const { return !(*this == rhs); }

        Mat2 operator+(const Mat2 &rhs) const
        {
            return Mat2(m00 + rhs.m00, m01 + rhs.m01,
                        m10 + rhs.m10, m11 + rhs.m11);
        }
        Mat2 operator-(const Mat2 &rhs) const
        {
            return Mat2(m00 - rhs.m00, m01 - rhs.m01,
                        m10 - rhs.m10, m11 - rhs.m11);
        }

        Mat2 &operator+=(const Mat2 &rhs)
        {
            m00 += rhs.m00, m01 += rhs.m01;
            m10 += rhs.m10, m11 += rhs.m11;
            return *this;
        }
        Mat2 &operator-=(const Mat2 &rhs)
        {
            m00 -= rhs.m00, m01 -= rhs.m01;
            m10 -= rhs.m10, m11 -= rhs.m11;
            return *this;
        }

        Mat2 operator*(Type rhs) const
        {
            return Mat2(m00 * rhs, m01 * rhs,
                        m10 * rhs, m11 * rhs);
        }
        Mat2 operator*=(Type rhs)
        {
            m00 *= rhs, m01 *= rhs;
            m10 *= rhs, m11 *= rhs;
            return *this;
        }
    };
}
#endif //!__AIRMAT2__H__