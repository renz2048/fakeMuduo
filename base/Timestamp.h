//
// Created by renz2048 on 2020/7/4.
//

#ifndef FAKEMUDUO_TIMESTAMP_H
#define FAKEMUDUO_TIMESTAMP_H

#include <inttypes.h>
#include <sys/time.h>

#include "Types.h"

namespace fakeMuduo {

    using std::string;

    class Timestamp {
    public:
        Timestamp()
                : microSecondsSinceEpoch_(0) {
        }

        /**
         * ����һ������ʱ��� Timestamp
         *
         * ʹ�� explicit ��ֹ��������ʽת��
         * @param microSecondsSinceEpochArg
         */
        explicit Timestamp(int64_t microSecondsSinceEpochArg)
                : microSecondsSinceEpoch_(microSecondsSinceEpochArg) {
        }

        /**
         * ���� 1970-00-00 00:00:00 ��΢������Ϊ�ǺϷ�ʱ��
         * @return
         */
        bool valid() const {
            return microSecondsSinceEpoch_ > 0;
        }

        int64_t microSecondsSinceEpoch() const {
            return microSecondsSinceEpoch_;
        }

        time_t secondsSinceEpoch() const {
            return static_cast<time_t>( microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        }

        void swap(Timestamp &that) {
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        }

        string toString() const; //const ���� this ָ�룬�������������޸����Ա����

        string toFormattedString(bool showMicroseconds = true) const;

        static Timestamp now();

        static Timestamp invalid() {
            return Timestamp();
        }

        static Timestamp fromUnixTime(time_t t) {
            return fromUnixTime(t, 0);
        }

        static Timestamp fromUnixTime(time_t t, int microseconds) {
            return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
        }

        static const int kMicroSecondsPerSecond = 1000 * 1000;
    private:
        int64_t microSecondsSinceEpoch_;
    };

    inline bool operator<(Timestamp lhs, Timestamp rhs) {
        return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
    }

    inline bool operator==(Timestamp lhs, Timestamp rhs) {
        return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
    }

/**
 * ����Ϊ��λ����ȡ����ʱ�����ʱ��
 * @param high
 * @param low
 * @return
 */
    inline double timeDifference(Timestamp high, Timestamp low) {
        int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
        return static_cast<double >(diff) / Timestamp::kMicroSecondsPerSecond;
    }
}

#endif //FAKEMUDUO_TIMESTAMP_H
