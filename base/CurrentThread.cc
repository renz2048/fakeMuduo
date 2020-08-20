//
// Created by renz2048 on 2020/7/9.
//

#include "CurrentThread.h"
#include <execinfo.h>
#include <cxxabi.h>

namespace fakeMuduo
{
    namespace CurrentThread
    {
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";
        /**
         * static_assert(bool_constexpr, message);
         * �����һ�������������ʽ bool_constexpr ��ֵΪ false�������һ���������
         * ����λ�þ��Ǹ� static_assert��������У��ڶ��������Ǵ�����ʾ�ַ���
         * ͨ������ abort ����ֹ��������
         *
         * ʹ�� static_assert�����ǿ����ڱ����ڼ䷢�ָ���Ĵ����ñ�������ǿ�Ʊ�֤һЩ��Լ
         * ������������һ�� static_assert ���ʱ��ͨ�����̽����һ��������Ϊ�������ʽ�������㣬
         * ������ó������ʽ������ĳЩģ����������ӳٵ�ģ��ʵ����ʱ�ٽ������㣬�ü��ģ�������Ϊ����
         */
        /**
         * std::is_same �ж������Ƿ�һ�£�����һ�������ͷ��� true
         */
        static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

        string stackTrace(bool demangle)
        {
            string stack;
            const int max_frames = 200;
            //���ڶ�ջ׷�ٵĵ�ַ���ݴ洢����
            void* frame[max_frames];
            //������ǰ��ջ����
            /*
             * int backtrace(void **buffer, int size);
             * ��ȡ��ǰ�̵߳ĺ������ö�ջ
             * ��ȡ����Ϣ����� buffer ��
             * ���� buffer ��ʵ�ʻ�ȡ�� void * ָ�����
             */
            int nptrs = ::backtrace(frame, max_frames);
            /*
             * char **backtrace_symbols(void *const *buffer, int size);
             * �ú����Ѵ� backtrace ������ȡ����Ϣ buffer ת��Ϊһ���ַ�������
             * ÿ���ַ������������������������ƫ�Ƶ�ַ��ʵ�ʵķ��ص�ַ
             */
            char** strings = ::backtrace_symbols(frame, max_frames);
            if (strings) {
                size_t len = 256;
                /*
                 * static_cast ��̬ת����ֻ�ܹ���������ת�������������޹�����֮���ת��
                 */
                char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
                for (int i = 1; i < nptrs; ++i) //skipping the 0-th, which is this function
                {
                    if (demangle) {
                        // https://panthema.net/2008/0901-stacktrace-demangled/
                        // ./module(_ZN3Bar4testEv+0x15c) [0x8048a6d]
                        char* left_par = nullptr;
                        char* plus = nullptr;
                        for (char* p = strings[i]; *p; ++p) {
                            if (*p == '(')
                                left_par = p;
                            else if (*p == '+')
                                plus = p;
                        }

                        if (left_par && plus) {
                            *plus = '\0';
                            int status = 0;
                            /*
                             * char* abi::__cxa_demangle(const char* mangled_name,
                             *                              char* output_buffer,
                             *                              size_t* length,
                             *                              int* status)
                             * ������
                             *  mangled_name���� NUL ��β���ַ���������Ҫ���ܵ�����
                             *  output_buffer��һ��ͨ��malloc������*length�ֽڵ��ڴ��������ڴ洢�ѷֽ������
                             *          ���output_buffer����������ʹ��realloc���������չ
                             *  length�����lengthΪ��NULL���򽫰����������ƵĻ������ĳ��ȷ���*length��
                             * status��
                             *  - 0�����������ɹ�
                             *  - -1���ڴ����ʧ��
                             *  - -2������ C++ ABI�޸Ĺ���mangled_name ������Ч����
                             *  - -3��������Ч
                             * ����ֵ��
                             *  ָ�룬ָ����NUL��ֹ���ѽ������ƵĿ�ͷ
                             *  ����ΪNULL���������ʧ��
                             * �����߸���ʹ��free�ͷŴ��ڴ�
                             */
                            char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);
                            *plus = '+';
                            if (status == 0) {
                                demangled = ret;
                                stack.append(strings[i], left_par+1);
                                stack.append(demangled);
                                stack.append(plus);
                                stack.push_back('\n');
                                continue;
                            }
                        }
                    }
                    stack.append(strings[i]);
                    stack.push_back('\n');
                }
                free(demangled);
                free(strings);
            }
            return stack;
        }
    }
}
