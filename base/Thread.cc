//
// Created by renz2048 on 2020/7/8.
//

#include "Thread.h"

namespace fakeMuduo
{
    namespace detail
    {
        pid_t gettid()
        {
            /**
             * syscall 执行一个系统调用，所使用的符号常量可以在头文件 <sys/syscall.h> 里面找到
             *
             * linux 中每个进程有一个 pid（pid_t），由 getpid() 取得。
             * linux 中 POSIX 线程 也有一个 id，类型 pthread_t，由 pthread_self() 取得，
             *   其 id 空间是各个进程独立的（即不同进程中的线程可能有相同的 id）。
             *   若进程P1 要向另外一个进程P2 中的某个线程发送信号，
             *   既不能使用 P2 的 pid，也不能使用线程的 pthread_id，
             *   只能使用线程的真实pid，即 tid
             *
             * tid 可以通过 gettid() 得到，但 glibc 并没有实现该函数，
             *   只能通过 linxu 的系统调用 syscall 来获取
             */
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void afterFork()
        {
        }
    }
    Thread::Thread(ThreadFunc, const string &name)
      : started_(false),
        joined_(false)
    {
        setDefaultName();
    }
}