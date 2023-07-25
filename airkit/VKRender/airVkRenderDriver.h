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
        VkRenderDriver();

    public:
        // 成员变量
        VkAllocationCallbacks *mAllocCall; // 内存分配器回调

        VkInstance mInstance;                  // 应用实例
        Vector<cstring> mInsExt;               // 实例扩展信息
        Vector<cstring> mLayers;               // 验证层扩展
        VkDebugReportCallbackEXT mDebugReport; // debug回调

        Vector<VkPhysicalDevice> mPhysicalSet;           // 物理设备集合
        VkPhysicalDevice mPhysical;                      // 选择的物理设备
        Vector<VkQueueFamilyProperties> mQueueFamilyPro; // 物理设备的队列属性集合

        // 需要的一些队列
        uint32 mGraphicsIndex; // 图形队列索引
        uint32 mComputeIndex;  // 计算队列索引
        uint32 mTransferIndex; // 传输队列索引

        VkQueue mGraphicsQueue; // 图形队列句柄
        VkQueue mComputeQueue;  // 计算队列句柄
        VkQueue mTransferQueue; // 传输队列句柄

        VkDevice mDevice;           // 逻辑设备
        Vector<cstring> mDeviceExt; // 逻辑设备扩展信息
        VkDescriptorPool mDescPool; // 描述池句柄

        
    public:
        void setup();
        // 释放
        void destory();

        static void checkVKResult(VkResult err);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
                                                          uint64_t object, size_t location, int32_t messageCode,
                                                          const char *pLayerPrefix, const char *pMessage, void *pUserData);
        // 通过呈现模式获取最小显示缓存图像数目
        void getMinImgCount();

    protected:
        // 检查验证层是否可用
        void checkLayerSupport();
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