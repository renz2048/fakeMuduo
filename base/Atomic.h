//
// Created by renz2048 on 2020/7/8.
//

#ifndef FAKEMUDUO_ATOMIC_H
#define FAKEMUDUO_ATOMIC_H

#include <cstdint>
#include "Types.h"

namespace fakeMuduo {
    namespace detail {
        template<typename T>
        class AtomicIntegerT {
        public:
            AtomicIntegerT()
                    : value_(0) {}

            T get() {
                return __sync_val_compare_and_swap(&value_, 0, 0);
            }

            T getAndAdd(T x) {
                return __sync_fetch_and_add(&value_, x);
            }

            T addAndGet(T x) {
                return getAndAdd(x) + x;
            }

            T incrementAndGet() {
                return addAndGet(1);
            }

            T decrementAndGet() {
                return addAndGet(-1);
            }

            void add(T x) {
                getAndAdd(x);
            }

            void increment() {
                incrementAndGet();
            }

            void decrement() {
                decrementAndGet();
            }

            T getAndSet(T newValue) {
                return __sync_lock_test_and_set(&value_, newValue);
            }

        private:
            //volatile修饰的变量，系统总是从新从内存中读取数据，而不是使用保存在寄存器中的备份
            volatile T value_;
        };
    } // namespace detail

    typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
} // namespace fakeMuduo

#endif //FAKEMUDUO_ATOMIC_H
