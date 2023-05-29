#ifndef __AIRATOMIC__H__
#define __AIRATOMIC__H__

namespace air
{
    // 原子性操作:支持1、2、4、8 整形或者指针
    template <typename Type>
    struct Atomic
    {
        // ins += value,返回 ins 旧值
        inline static Type fetchAdd(volatile Type &ins, Type value) { return __sync_fetch_and_add(&ins, value); }
        // ins += value,返回 ins 新值
        inline static Type addFetch(volatile Type &ins, Type value) { return __sync_add_and_fetch(&ins, value); }

        // ins -= value,返回 ins 旧值
        inline static Type fetchSub(volatile Type &ins, Type value) { return __sync_fetch_and_sub(&ins, value); }
        // ins -= value,返回 ins 新值
        inline static Type subFetch(volatile Type &ins, Type value) { return __sync_sub_and_fetch(&ins, value); }

        // ins &= value,返回 ins 旧值
        inline static Type fetchAnd(volatile Type &ins, Type value) { return __sync_fetch_and_and(&ins, value); }
        // ins &= value,返回 ins 新值
        inline static Type andFetch(volatile Type &ins, Type value) { return __sync_and_and_fetch(&ins, value); }

        // ins |= value,返回 ins 旧值
        inline static Type fetchOr(volatile Type &ins, Type value) { return __sync_fetch_and_or(&ins, value); }
        // ins |= value,返回 ins 新值
        inline static Type orFetch(volatile Type &ins, Type value) { return __sync_or_and_fetch(&ins, value); }

        // ins ^= value,返回 ins 旧值
        inline static Type fetchXor(volatile Type &ins, Type value) { return __sync_fetch_and_xor(&ins, value); }
        // ins ^= value,返回 ins 新值
        inline static Type xorFetch(volatile Type &ins, Type value) { return __sync_xor_and_fetch(&ins, value); }

        // ins = ~ins & value,返回 ins 旧值
        inline static Type fetchNAnd(volatile Type &ins, Type value) { return __sync_fetch_and_nand(&ins, value); }
        // ins = ~ins & value,返回 ins 新值
        inline static Type nandFetch(volatile Type &ins, Type value) { return __sync_nand_and_fetch(&ins, value); }
        // if(ins ==oldvalue) ins=newvalue,return true; else return false;
        inline static bool boolCompareSwap(volatile Type &ins, Type oldvalue, Type newvalue) { return __sync_bool_compare_and_swap(&ins, oldvalue, newvalue); }
        // if(ins ==oldvalue) ins=newvalue,return oldvalue; else return ins 旧值;
        inline static bool compareSwap(volatile Type &ins, Type oldvalue, Type newvalue) { return __sync_bool_compare_and_swap(&ins, oldvalue, newvalue); }
    };

}
#endif //!__AIRATOMIC__H__