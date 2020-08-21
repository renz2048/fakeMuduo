//
// Created by renz2048 on 2020/8/20.
//

#include <string>
#include <stdio.h>
#include <unistd.h>

#include "Thread.h"
#include "CurrentThread.h"

void threadFunc()
{
    // CurrentThread::tid() -> CurrentThread::t_cachedTid -> CurrentThread::cachedTid()
    //                         detail::gettid() -> syscall(SYS_gettid)
    printf("tid=%d\n", fakeMuduo::CurrentThread::tid());
}

int main()
{
    printf("pid=%d, tid=%d\n", ::getpid(), fakeMuduo::CurrentThread::tid());

    fakeMuduo::Thread t1(threadFunc);
    t1.start();
    printf("t1.tid=%d\n", t1.tid());
    t1.join();
}

