# 获得当前目录名
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})

# 获取 main.c 文件
file(GLOB_RECURSE main_file ./*main.c)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加第三方库(add)头文件路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#生成可执行文件
ADD_EXECUTABLE(${CURRENT_FOLDER}_exe ${main_file})

#添加链接库
execute_process(
	COMMAND ls ${PROJECT_SOURCE_DIR}/lib
	OUTPUT_VARIABLE libs
	)
string(REPLACE "\n" ";" libList ${libs})
foreach(item ${libList})
	message("add lib: ${item}")
	TARGET_LINK_LIBRARIES(${CURRENT_FOLDER}_exe ${CURRENT_FOLDER}/lib/${item})
endforeach()

