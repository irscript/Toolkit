#ifndef __AIRMAT4__H__
#define __AIRMAT4__H__
#include <airkit/Core/airTypes.h>
#include <airkit/Math/airMathDef.h>
#include <airkit/Struct/airString.h>
namespace air
{
    template <typename Type>
    struct Mat4
    {
    public:
        union
        {
            Type mArr[4 * 4];
            struct
            {
                Type m00, m01, m02, m03;
                Type m10, m11, m12, m13;
                Type m20, m21, m22, m23;
                Type m30, m31, m32, m33;
            };
        };
        // 单位矩阵
        Mat4() : m00(1), m01(0), m02(0), m03(0),
                 m10(0), m11(1), m12(0), m13(0),
                 m20(0), m21(0), m22(1), m23(0),
                 m30(0), m31(0), m32(0), m33(1) {}

        Mat4(Type v)
            : m00(v), m01(0), m02(0), m03(0),
              m10(0), m11(v), m12(0), m13(0),
              m20(0), m21(0), m22(v), m23(0),
              m30(0), m31(0), m32(0), m33(v) {}

        Mat4(const Mat4 &rhs)
            : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(rhs.m03),
              m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(rhs.m13),
              m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(rhs.m23),
              m30(rhs.m30), m31(rhs.m31), m32(rhs.m32), m33(rhs.m33) {}

        Mat4(Type v00, Type v01, Type v02, Type v03,
             Type v10, Type v11, Type v12, Type v13,
             Type v20, Type v21, Type v22, Type v23,
             Type v30, Type v31, Type v32, Type v33)
            : m00(v00), m01(v01), m02(v02), m03(v03),
              m10(v10), m11(v11), m12(v12), m13(v13),
              m20(v20), m21(v21), m22(v22), m23(v23),
              m30(v30), m31(v31), m32(v32), m33(v33) {}

        Mat4(Type *v)
            : m00(v[0]), m01(v[1]), m02(v[2]), m03(v[3]),
              m10(v[4]), m11(v[5]), m12(v[6]), m13(v[7]),
              m20(v[8]), m21(v[9]), m22(v[10]), m23(v[11]),
              m30(v[12]), m31(v[13]), m32(v[14]), m33(v[15]) {}

        bool equals(const Mat4 &rhs) const
        {
            if (Math::equals(m00, rhs.m00) == false)
                return false;
            if (Math::equals(m01, rhs.m01) == false)
                return false;
            if (Math::equals(m02, rhs.m02) == false)
                return false;
            if (Math::equals(m03, rhs.m03) == false)
                return false;

            if (Math::equals(m10, rhs.m10) == false)
                return false;
            if (Math::equals(m11, rhs.m11) == false)
                return false;
            if (Math::equals(m12, rhs.m12) == false)
                return false;
            if (Math::equals(m13, rhs.m13) == false)
                return false;

            if (Math::equals(m20, rhs.m20) == false)
                return false;
            if (Math::equals(m21, rhs.m21) == false)
                return false;
            if (Math::equals(m22, rhs.m22) == false)
                return false;
            if (Math::equals(m23, rhs.m23) == false)
                return false;

            if (Math::equals(m30, rhs.m30) == false)
                return false;
            if (Math::equals(m31, rhs.m31) == false)
                return false;
            if (Math::equals(m32, rhs.m32) == false)
                return false;
            if (Math::equals(m33, rhs.m33) == false)
                return false;

            return true;
        }

        // 重载操作数
        Mat4 &operator=(const Mat4 &rhs)
        {
            m00 = rhs.m00, m01 = rhs.m01, m02 = rhs.m02, m03 = rhs.m03;
            m10 = rhs.m10, m11 = rhs.m11, m12 = rhs.m12, m13 = rhs.m13;
            m20 = rhs.m20, m21 = rhs.m21, m22 = rhs.m22, m23 = rhs.m23;
            m30 = rhs.m30, m31 = rhs.m31, m32 = rhs.m32, m33 = rhs.m33;
            return *this;
        }
        bool operator==(const Mat4 &rhs) const
        {
            if (m00 != rhs.m00)
                return false;
            if (m01 != rhs.m01)
                return false;
            if (m02 != rhs.m02)
                return false;
            if (m03 != rhs.m03)
                return false;

            if (m10 != rhs.m10)
                return false;
            if (m11 != rhs.m11)
                return false;
            if (m12 != rhs.m12)
                return false;
            if (m13 != rhs.m13)
                return false;

            if (m20 != rhs.m20)
                return false;
            if (m21 != rhs.m21)
                return false;
            if (m22 != rhs.m22)
                return false;
            if (m23 != rhs.m23)
                return false;

            if (m30 != rhs.m30)
                return false;
            if (m31 != rhs.m31)
                return false;
            if (m32 != rhs.m32)
                return false;
            if (m33 != rhs.m33)
                return false;

            return true;
        }
        bool operator!=(const Mat4 &rhs) const { return !(*this == rhs); }

        Mat4 operator+(const Mat4 &rhs) const
        {
            return Mat4(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
                        m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                        m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                        m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33);
        }
        Mat4 operator-(const Mat4 &rhs) const
        {
            return Mat4(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
                        m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                        m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                        m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33);
        }

        Mat4 &operator+=(const Mat4 &rhs)
        {
            m00 += rhs.m00, m01 += rhs.m01, m02 += rhs.m02, m03 += rhs.m03;
            m10 += rhs.m10, m11 += rhs.m11, m12 += rhs.m12, m13 += rhs.m13;
            m20 += rhs.m20, m21 += rhs.m21, m22 += rhs.m22, m23 += rhs.m23;
            m30 += rhs.m30, m31 += rhs.m31, m32 += rhs.m32, m33 += rhs.m33;
            return *this;
        }
        Mat4 &operator-=(const Mat4 &rhs)
        {
            m00 -= rhs.m00, m01 -= rhs.m01, m02 -= rhs.m02, m03 -= rhs.m03;
            m10 -= rhs.m10, m11 -= rhs.m11, m12 -= rhs.m12, m13 -= rhs.m13;
            m20 -= rhs.m20, m21 -= rhs.m21, m22 -= rhs.m22, m23 -= rhs.m23;
            m30 -= rhs.m30, m31 -= rhs.m31, m32 -= rhs.m32, m33 -= rhs.m33;
            return *this;
        }

        Mat4 operator*(Type rhs) const
        {
            return Mat4(m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
                        m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
                        m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
                        m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs);
        }
        Mat4 operator*=(Type rhs)
        {
            m00 *= rhs, m01 *= rhs, m02 *= rhs, m03 *= rhs;
            m10 *= rhs, m11 *= rhs, m12 *= rhs, m13 *= rhs;
            m20 *= rhs, m21 *= rhs, m22 *= rhs, m23 *= rhs;
            m30 *= rhs, m31 *= rhs, m32 *= rhs, m33 *= rhs;
            return *this;
        }
    };
    // 正交投影
    template <typename Type>
    inline Mat4<Type> Ortho(Type top, Type bottom,
                            Type left, Type right,
                            Type nears, Type far)
    {
        Mat4<Type> result;
        result.m00 = real(2) / (right - left);
        result.m11 = real(2) / (top - bottom);
        result.m22 = real(2) / (far - nears);
        result.m30 = -(right + left) / (right - left);
        result.m31 = -(top + bottom) / (top - bottom);
        result.m32 = -(far + nears) / (far - nears);
        return result;
    }
}
#endif //!__AIRMAT4__H__