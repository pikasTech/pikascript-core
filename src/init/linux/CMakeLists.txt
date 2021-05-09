# 获得当前目录名
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})

# 获取 main.c 文件
file(GLOB_RECURSE main_file ./*main.c*)

# 获取.a文件
file(GLOB_RECURSE lib_file ${PROJECT_SOURCE_DIR}/lib/*.a) 

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# set link options
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
message ("link options: " ${CMAKE_EXE_LINKER_FLAGS})

# 添加第三方库(add)头文件路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#生成可执行文件
ADD_EXECUTABLE(${CURRENT_FOLDER}.bin ${main_file})

TARGET_LINK_LIBRARIES(${CURRENT_FOLDER}.bin ${lib_file})

# 添加链接库
#execute_process(
	#COMMAND ls ${PROJECT_SOURCE_DIR}/lib
	#OUTPUT_VARIABLE libs
	#	)
#string(REPLACE "\n" ";" libList ${libs})
#foreach(item ${libList})
	#message("add lib: ${item}")
	#TARGET_LINK_LIBRARIES(${CURRENT_FOLDER}_exe ${PROJECT_SOURCE_DIR}/lib/${item})
	#endforeach()

