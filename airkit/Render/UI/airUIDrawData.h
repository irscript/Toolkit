#ifndef __AIRUIDRAWDATA__H__
#define __AIRUIDRAWDATA__H__

#include <airkit/Struct/airVector.h>
#include <airkit/Math/airMath.h>
#include <airkit/Render/airColor.h>
#include <airkit/Struct/airList.h>

namespace air
{
    // UI 顶点数据的索引
    using UIIndex = uint16;
    // UI 顶点描述
    struct UIVectex
    {
        Vec2f mPos;  // 顶点坐标
        Vec2f mUVW;  // 纹理坐标
        RGBA32 mClr; // 顶点颜色
    };
    // 绘制命令
    struct UIDrawCMD
    {
        uintptr mTex;    // 纹理句柄
        Vec4f mClip;     // 裁剪矩形
        UIIndex mOffset; // 索引偏移
        UIIndex mCount;  // 索引数量
    };
    // 绘制列表
    struct UIDrawList
    {
        Vector<UIVectex> mVetex; // 顶点缓存
        Vector<UIIndex> mIndex;  // 索引缓存
        // 具体的绘制控制
        List<UIDrawCMD> mDraw; // 绘制列表
    };
}

#endif //!__AIRUIDRAWDATA__H__