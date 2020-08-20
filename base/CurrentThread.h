//
// Created by renz2048 on 2020/7/9.
//

#ifndef FAKEMUDUO_CURRENTTHREAD_H
#define FAKEMUDUO_CURRENTTHREAD_H

#include "Types.h"

#include <pthread.h>

namespace fakeMuduo
{
    /**
     * 用于获取当前线程的一些状态
     * 如：
     * 线程真实 tid
     * 以字符串格式存储的真实 tid 名称
     */
    namespace CurrentThread
    {
        // internal
        /**
         * Thread local Storage（线程局部存储）机制分配的变量
         * 每个当前线程有一个该 __thread 变量的实例
         */
        extern __thread int t_cachedTid;            //当前线程 pid
        extern __thread char t_tidString[32];       //当前线程 pid 的字符串格式
        extern __thread int t_tidStringLength;
        extern __thread const char* t_threadName;   //

        /**
         * 获取当前线程的pid，存放在 t_cachedTid
         * 转换为字符串格式，存放在t_tidString
         * 长度存放在t_tidStringLength
         */
        void cacheTid();

        inline int tid()
        {
            /**
             * long __builtin_expect(long exp, long c);
             * c = 0，表示认为执行if语句的可能性小，即认为 t_cachedTid 大部分情况都不会等于0
             * 编译的时候优先安排if之后的代码
             */
            if (__builtin_expect(t_cachedTid == 0, 0)) {
                cacheTid();
            }
            return t_cachedTid;
        }

        inline const char* tidString() {
            return t_tidString;
        }

        inline int tidStringLength() {
            return t_tidStringLength;
        }

        inline const char* name() {
            return t_threadName;
        }

        /**
         * 当前线程的真实pid由tid()获得，getpid获得主线程的pid
         * @return
         */
        bool isMainThread();

        /**
         * 暂停调用线程的执行，知道 usec 时间后恢复
         * 也可能因为其它信号触发了线程中的处理程序的调用
         */
        void sleepUsec(int64_t usec);

        /**
         * 获取当前线程的函数调用堆栈
         * @param demangle
         * @return
         */
        string stackTrace(bool demangle);
    } // namespace CurrentThread
} // namespace fakeMuduo


#endif //FAKEMUDUO_CURRENTTHREAD_H
