#ifndef __AIRHASH32__H__
#define __AIRHASH32__H__
#include <airkit/Core/airTypes.h>

namespace air::Hash
{

    // 哈希函数原型
    using Hash32Func = uint32 (*)(const uint8 *, uint);

    /**
     * @brief RS 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 RS(const uint8 *aData, uint nSize);
    /**
     * @brief JS 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 JS(const uint8 *aData, uint nSize);
    /**
     * @brief PJW 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 PJW(const uint8 *aData, uint nSize);

    /**
     * @brief ELF 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 ELF(const uint8 *aData, uint nSize);

    /**
     * @brief BKDR 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 BKDR(const uint8 *aData, uint nSize);
    /**
     * @brief SDBM 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 SDBM(const uint8 *aData, uint nSize);
    /**
     * @brief DJB 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 DJB(const uint8 *aData, uint nSize);
    /**
     * @brief DEK 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 DEK(const uint8 *aData, uint nSize);

    /**
     * @brief DEK 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 AP(const uint8 *aData, uint nSize);
    /**
     * @brief  BP 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 BP(const uint8 *aData, uint nSize);
    /**
     * @brief DEK 哈希函数
     * @param aData 待哈希运行的数据
     * @param nSize 数据得长度，单位字节
     * @return 哈希值
     */
    uint32 FNV(const uint8 *aData, uint nSize);
}
#endif //!__AIRHASH__H__