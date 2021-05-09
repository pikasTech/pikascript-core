cmake_minimum_required(VERSION 3.10)

#设置库文件输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

#设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# 添加子目录
add_subdirectory(add)
add_subdirectory(sub)
add_subdirectory(example)
