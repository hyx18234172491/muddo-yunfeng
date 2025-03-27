#pragma once

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{
    extern __thread int t_cachedTid; // 保存tid缓存 因为系统调用非常耗时 拿到tid后将其保存

    void cacheTid();

    inline int tid() // 内联函数只在当前文件中起作用
    {
        if (__builtin_expect(t_cachedTid == 0, 0)) // __builtin_expect 是一种底层优化 此语句意思是如果还未获取tid 进入if 通过cacheTid()系统调用获取tid
        {
            // 是 GCC 的分支预测优化：这里 0 表示 几乎不会发生（即 t_cachedTid != 0 更常见）。
            // 这样，CPU 预取指令时 更倾向于跳过 if 语句，优化执行效率
            cacheTid();
        }
        return t_cachedTid;
    }
}