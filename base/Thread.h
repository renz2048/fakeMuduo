//
// Created by renz2048 on 2020/7/8.
//

#ifndef FAKEMUDUO_THREAD_H
#define FAKEMUDUO_THREAD_H

#include "Atomic.h"
#include <functional>
#include <zconf.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace fakeMuduo {
    class Thread {
    public:
        typedef std::function<void ()> ThreadFunc;
        explicit Thread(ThreadFunc, const string& name = string());
        ~Thread();

        void start();
        int join();

        bool started() const { return started_; }
        pid_t tid() const { return tid_; }
        const string& name() const { return name_; }

        static int numCreated() { return numCreated_.get(); }
    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        pthread_t pthreadId_; // 线程 id
        pid_t tid_; // 线程真实 id
        ThreadFunc func_;
        string name_;

        static AtomicInt32 numCreated_;
    };
} // namespace fakeMuduo

#endif //FAKEMUDUO_THREAD_H
