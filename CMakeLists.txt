cmake_minimum_required(VERSION 3.16)
project(fakeMuduo)
set(CMAKE_CXX_STANDARD 14)

# 添加base 头文件库到头文件搜索路径
include_directories("${PROJECT_SOURCE_DIR}/base")

# 添加 library fakeMuduo_base 库到 fakeMuduo
#target_link_libraries(fakeMuduo fakeMuduo_base)

#install(TARGETS fakeMuduo DESTINATION bin)

add_subdirectory(base)