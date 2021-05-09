# 获得当前目录名
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})

# 设置版本
cmake_minimum_required(VERSION 3.10)

# 设置工程名为当前目录名
project(${CURRENT_FOLDER})

# 设置全局宏
add_definitions(-D LINUX)

#设置库文件输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

#设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# 添加子目录
add_subdirectory(mimiapp)
add_subdirectory(mimidata)
add_subdirectory(mimimemory)
add_subdirectory(mimiport)
add_subdirectory(mimishell)
add_subdirectory(mimistr)
add_subdirectory(mimitest)
add_subdirectory(mimivm)
