//
// Created by renz2048 on 2020/7/8.
//
#include <assert.h>
#include <iostream>
#include "Atomic.h"

int main()
{
    std::cout << "hello Atomic_unittest.cc" << std::endl;
    {
        fakeMuduo::AtomicInt64 a0;
        assert(a0.get() == 0);
        assert(a0.getAndAdd(1) == 0);
        assert(a0.get() == 1);
        assert(a0.addAndGet(2) == 3);
        assert(a0.get() == 3);
        assert(a0.incrementAndGet() == 4);
        assert(a0.get() == 4);
        a0.increment();
        assert(a0.get() == 5);
        assert(a0.addAndGet(-3) == 2);
        assert(a0.getAndSet(100) == 2);
        assert(a0.get() == 100);
    }
    std::cout << "tail Atomic_unittest.cc" << std::endl;
}
