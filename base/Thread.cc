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
             * syscall ִ��һ��ϵͳ���ã���ʹ�õķ��ų���������ͷ�ļ� <sys/syscall.h> �����ҵ�
             *
             * linux ��ÿ��������һ�� pid��pid_t������ getpid() ȡ�á�
             * linux �� POSIX �߳� Ҳ��һ�� id������ pthread_t���� pthread_self() ȡ�ã�
             *   �� id �ռ��Ǹ������̶����ģ�����ͬ�����е��߳̿�������ͬ�� id����
             *   ������P1 Ҫ������һ������P2 �е�ĳ���̷߳����źţ�
             *   �Ȳ���ʹ�� P2 �� pid��Ҳ����ʹ���̵߳� pthread_id��
             *   ֻ��ʹ���̵߳���ʵpid���� tid
             *
             * tid ����ͨ�� gettid() �õ����� glibc ��û��ʵ�ָú�����
             *   ֻ��ͨ�� linxu ��ϵͳ���� syscall ����ȡ
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