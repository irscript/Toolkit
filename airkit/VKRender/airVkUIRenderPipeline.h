#ifndef __AIRVKUIRENDERPIPELINE__H__
#define __AIRVKUIRENDERPIPELINE__H__
#include <airkit/Render/airRender.h>
#include <vulkan/vulkan.h>
#include <airkit/VKRender/airVkRenderDriver.h>
namespace air
{

    struct VkUIPipeline : public IUIRenderPipeline
    {
        // 渲染窗口
        virtual void onRenderWindow(IWindow *window) override;

    protected:
        friend struct VkRender;
        VkSampler mUISamper;               // UI纹理采样器（包括字体）
        VkDescriptorSetLayout mDescLayout; // 着色器常量布局
        VkPipelineLayout mPipelineLayout;  // 管线布局
        VkPipelineCache mPipelineCache;    // 管线缓存
        VkPipeline mPipeline;              // 管线
        VkShaderModule mVertShader;        // 顶点着色器
        VkShaderModule mFragShader;        // 片段着色器

        // 创建采样器
        void createSamper(VkRenderDriver &driver);
        // 创建描述集布局
        void creatDescLayout(VkRenderDriver &driver);
        // 创建管线布局
        void createPipelineLayout(VkRenderDriver &driver);
        // 创建着色器
        void createShaderModule(VkRenderDriver &driver);
        // 创建管线
        void createPipeline(VkRenderDriver &driver);

        // 释放管线
        void destoryPipeline(VkRenderDriver &driver);
    };
}
#endif //!__AIRVKUIRENDERPIPELINE__H__