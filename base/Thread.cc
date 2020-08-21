//
// Created by renz2048 on 2020/7/8.
//

#include "Thread.h"
#include "CurrentThread.h"
#include "Timestamp.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

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
            fakeMuduo::CurrentThread::t_cachedTid = 0;
            fakeMuduo::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer
        {
        public:
            ThreadNameInitializer()
            {
                fakeMuduo::CurrentThread::t_threadName = "main";
                CurrentThread::tid();
                pthread_atfork(NULL, NULL, &afterFork);
            }
        };

        ThreadNameInitializer init;

        struct ThreadData
        {
            typedef fakeMuduo::Thread::ThreadFunc ThreadFunc;
            ThreadFunc func_;
            string name_;
            pid_t* tid_;

            ThreadData(ThreadFunc func,
                       const string& name,
                       pid_t* tid)
                       : func_(std::move(func)),
                         name_(name),
                         tid_(tid)
            {}

            void runInThread()
            {
                fakeMuduo::CurrentThread::t_threadName = name_.empty() ? "fakeMuduoThread" : name_.c_str();
                /*
                 * #include <sys/prctl.h>
                 * int prctl(int option, unsigned long arg2, unsinged long arg3, unsigned long arg4, unsigned long arg5);
                 * PR_SET_NAME�����ý�����
                 */
                ::prctl(PR_SET_NAME, fakeMuduo::CurrentThread::t_threadName);

                func_();
                fakeMuduo::CurrentThread::t_threadName = "finished";
            }
        };

        void* startThread(void* obj)
        {
            ThreadData* data = static_cast<ThreadData*>(obj);
            data->runInThread();
            delete data;
            return NULL;
        }
    } // namespace detail

    /**
     * ��ȡ��ǰ�̵߳�pid������� t_cachedTid
     * ת��Ϊ�ַ�����ʽ�������t_tidString
     * ���ȴ����t_tidStringLength
     */
    void CurrentThread::cacheTid() {
        if (t_cachedTid == 0) {
            t_cachedTid = detail::gettid();
            t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        }
    }

    /**
     * ��ǰ�̵߳���ʵpid��tid()��ã�getpid������̵߳�pid
     * @return
     */
    bool CurrentThread::isMainThread() {
        return tid() == ::getpid();
    }

    /**
     * ��ͣ�����̵߳�ִ�У�֪�� usec ʱ���ָ�
     * Ҳ������Ϊ�����źŴ������߳��еĴ������ĵ���
     */
    void CurrentThread::sleepUsec(int64_t usec) {
        struct timespec ts = {0, 0};
        ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
        ::nanosleep(&ts, NULL);
    }

    Thread::Thread(ThreadFunc func, const string &n)
      : started_(false),
        joined_(false),
        pthreadId_(0),
        tid_(0),
        func_(std::move(func)),
        name_(n)
    {
        setDefaultName();
    }

    void Thread::setDefaultName() {
        int num = numCreated_.incrementAndGet();
        if (name_.empty()) {
            char buf[32];
            snprintf(buf, sizeof buf, "Thread%d", num);
            name_ = buf;
        }
    }

    void Thread::start() {
        assert(!started_);
        started_ = true;
        detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_);
        if (pthread_create(&pthreadId_, NULL, &detail::startThread, data)) {
            started_ = false;
            delete data;
        } else {
            assert(tid_ > 0);
        }
    }

    int Thread::join() {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, NULL);
    }
}