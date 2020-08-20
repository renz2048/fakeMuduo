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
     * ���ڻ�ȡ��ǰ�̵߳�һЩ״̬
     * �磺
     * �߳���ʵ tid
     * ���ַ�����ʽ�洢����ʵ tid ����
     */
    namespace CurrentThread
    {
        // internal
        /**
         * Thread local Storage���ֲ߳̾��洢�����Ʒ���ı���
         * ÿ����ǰ�߳���һ���� __thread ������ʵ��
         */
        extern __thread int t_cachedTid;            //��ǰ�߳� pid
        extern __thread char t_tidString[32];       //��ǰ�߳� pid ���ַ�����ʽ
        extern __thread int t_tidStringLength;
        extern __thread const char* t_threadName;   //

        /**
         * ��ȡ��ǰ�̵߳�pid������� t_cachedTid
         * ת��Ϊ�ַ�����ʽ�������t_tidString
         * ���ȴ����t_tidStringLength
         */
        void cacheTid();

        inline int tid()
        {
            /**
             * long __builtin_expect(long exp, long c);
             * c = 0����ʾ��Ϊִ��if���Ŀ�����С������Ϊ t_cachedTid �󲿷�������������0
             * �����ʱ�����Ȱ���if֮��Ĵ���
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
         * ��ǰ�̵߳���ʵpid��tid()��ã�getpid������̵߳�pid
         * @return
         */
        bool isMainThread();

        /**
         * ��ͣ�����̵߳�ִ�У�֪�� usec ʱ���ָ�
         * Ҳ������Ϊ�����źŴ������߳��еĴ������ĵ���
         */
        void sleepUsec(int64_t usec);

        /**
         * ��ȡ��ǰ�̵߳ĺ������ö�ջ
         * @param demangle
         * @return
         */
        string stackTrace(bool demangle);
    } // namespace CurrentThread
} // namespace fakeMuduo


#endif //FAKEMUDUO_CURRENTTHREAD_H
