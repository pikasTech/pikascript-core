set(SRC_DIR src/kernal)

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

# 添加子目录
execute_process(
	COMMAND ls ${PROJECT_SOURCE_DIR}/${SRC_DIR}
	OUTPUT_VARIABLE subDirs
	)

string(REPLACE "\n" ";" subDirsList ${subDirs})

#编译下一级所有子模块
foreach(item ${subDirsList})
	message("add sub directory: ${SRC_DIR}/${item}")
	add_subdirectory(${SRC_DIR}/${item})
endforeach()

