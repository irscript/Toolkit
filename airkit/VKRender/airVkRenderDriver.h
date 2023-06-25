#ifndef __AIRVKRENDERDRIVER__H__
#define __AIRVKRENDERDRIVER__H__

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <airkit/Render/airRender.h>

namespace air
{
    // 渲染器设备驱动
    struct VkRenderDriver
    {
        Vector<VkPhysicalDevice> mPhysicalset; // 物理设备集合

        // VkAllocationCallbacks mAllocCallback; // 内存分配
        VkAllocationCallbacks *mACallback; // 内存分配
        VkInstance mInstance;              // 实例句柄
        VkPhysicalDevice mPhysical;        // 选择的物理设备
        VkDevice mDevice;                  // 逻辑设备句柄
        VkDescriptorPool mDescPool;        // 描述池

        Vector<cstring> mPhysExt;                        // 开启的扩展信息
        Vector<VkQueueFamilyProperties> mQueueFamilyPro; // 队列簇属性
        Vector<cstring> mDeviceExt;                      // 开启的逻辑设备扩展

        // 显示表面相关信息

        // 队列索引
        int32 mGraphicsIndex; // 图像队列索引
        int32 mPresentIndex;  // 呈现队列索引
        int32 mComputeIndex;  // 计算队列索引
        int32 mTransferIndex; // 传输队列索引

        // 队列句柄
        VkQueue mGraphicsQueue; // 图像队列句柄
        VkQueue mPresentQueue;  // 呈现队列句柄
        VkQueue mComputeQueue;  // 计算队列句柄
        VkQueue mTransferQueue; // 传输队列句柄

#ifdef _air_vulkan_debug_report
        VkDebugReportCallbackEXT mReportCallback; // 错误回调
        Vector<cstring> mLayers;                  // 验证层
#endif

        // 通过扩展启动渲染器设备驱动
        void setup();
        // 释放
        void destory();

        static void check_vk_result(VkResult err);

    protected:
        // 创建实例
        void createInstance();
        // 选择gpu
        void selectGPU();
        // 选择队列簇
        void selectQueueFamily();
        // 创建逻辑设备
        void createDeveice();
        // 创建描述池
        void createDescriptorPool();
    };
}
#endif //!__AIRVKRENDERDRIVER__H__