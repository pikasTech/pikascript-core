set(SRC_DIR src)

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
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#编译下一级所有子模块----kernal
	add_subdirectory(${SRC_DIR}/kernal)

# 添加子目录---init
execute_process(
	COMMAND ls ${PROJECT_SOURCE_DIR}/${SRC_DIR}/init
	OUTPUT_VARIABLE subDirs_init
	)
string(REPLACE "\n" ";" subDirsList_init ${subDirs_init})

#编译下一级所有子模块----init
foreach(item ${subDirsList_init})
	message("add sub directory: ${SRC_DIR}/${item}")
	add_subdirectory(${SRC_DIR}/init/${item})
endforeach()
