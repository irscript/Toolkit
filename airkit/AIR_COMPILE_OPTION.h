#ifndef __AIR_COMPILE_OPTION__H__
#define __AIR_COMPILE_OPTION__H__

// 编译选项在此定义

// 内存泄露检查宏
#ifndef _check_memory_free
#define _check_memory_free
#endif

// 确保表达式成立
#ifndef _air_no_debug
#define make_ensure(exp)                                             \
    if (!(exp))                                                      \
    {                                                                \
        air::assertion(#exp, this_file(), this_func(), this_line()); \
    }
#else
#define make_ensure(exp)
#endif //_air_no_debug

// 展开循环优化
#define opt_unroll __attribute__((__optimize__("-funroll-loops")))

#endif //!__AIR_COMPILE_OPTION__H__