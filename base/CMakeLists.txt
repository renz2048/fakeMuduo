set(base_SRCS Timestamp.cc Thread.cc CurrentThread.cc)

# 生成链接库
add_library(fakeMuduo_base ${base_SRCS})

# 添加链接库
target_link_libraries(fakeMuduo_base pthread)

install(TARGETS fakeMuduo_base DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/base)

# 添加 test 子目录
add_subdirectory(tests)