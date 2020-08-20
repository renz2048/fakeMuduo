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
         * 如果第一个参数常量表达式 bool_constexpr 的值为 false，会产生一条编译错误，
         * 错误位置就是该 static_assert语句所在行；第二个参数是错误提示字符串
         * 通过调用 abort 来终止程序运行
         *
         * 使用 static_assert，我们可以在编译期间发现更多的错误，用编译器来强制保证一些契约
         * 编译器在遇到一个 static_assert 语句时，通常立刻将其第一个参数作为常量表达式进行演算，
         * 但如果该常量表达式依赖于某些模板参数，则延迟到模板实例化时再进行演算，让检查模板参数成为可能
         */
        /**
         * std::is_same 判断类型是否一致，两个一样的类型返回 true
         */
        static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

        string stackTrace(bool demangle)
        {
            string stack;
            const int max_frames = 200;
            //用于堆栈追踪的地址数据存储数组
            void* frame[max_frames];
            //检索当前堆栈数组
            /*
             * int backtrace(void **buffer, int size);
             * 获取当前线程的函数调用堆栈
             * 获取的信息存放在 buffer 中
             * 返回 buffer 中实际获取的 void * 指针个数
             */
            int nptrs = ::backtrace(frame, max_frames);
            /*
             * char **backtrace_symbols(void *const *buffer, int size);
             * 该函数把从 backtrace 函数获取的信息 buffer 转化为一个字符串数组
             * 每个字符串数组包含函数名、函数的偏移地址和实际的返回地址
             */
            char** strings = ::backtrace_symbols(frame, max_frames);
            if (strings) {
                size_t len = 256;
                /*
                 * static_cast 静态转换；只能够用于良性转换，不能用于无关类型之间的转换
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
                             * 参数：
                             *  mangled_name：以 NUL 结尾的字符串，包含要解密的名称
                             *  output_buffer：一块通过malloc分配了*length字节的内存区域，用于存储已分解的名称
                             *          如果output_buffer不够长，则使用realloc对其进行扩展
                             *  length：如果length为非NULL，则将包含解析名称的缓冲区的长度放在*length中
                             * status：
                             *  - 0：解析操作成功
                             *  - -1：内存分配失败
                             *  - -2：根据 C++ ABI修改规则，mangled_name 不是有效名称
                             *  - -3：参数无效
                             * 返回值：
                             *  指针，指向以NUL终止的已解析名称的开头
                             *  或者为NULL，如果解析失败
                             * 调用者负责使用free释放次内存
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
