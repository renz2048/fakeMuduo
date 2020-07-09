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
    private:
        void setDefaultName();
        bool started_;
        bool joined_;
    };
}

#endif //FAKEMUDUO_THREAD_H
