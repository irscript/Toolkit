#ifndef __AIR_COMPILE_OPTION__H__
#define __AIR_COMPILE_OPTION__H__

// 编译选项在此定义

// 内存泄露检查宏
#ifndef _check_memory_free
#define _check_memory_free
#endif

// 确保表达式成立
#ifndef _air_no_debug
#define ensure(exp) assert(exp)
#else
#define ensure(exp)
#endif //_air_no_debug

#endif //!__AIR_COMPILE_OPTION__H__