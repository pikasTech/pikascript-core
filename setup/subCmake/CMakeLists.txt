# 获得当前目录名
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})

# 递归获取目录下所有的C文件
file(GLOB_RECURSE c_files ./*.c)

# 递归获取目录下所有的h文件
file(GLOB_RECURSE h_files ./*.h)

#将.h文件复制到根目录的include文件夹
file(COPY ${h_files}
DESTINATION ${PROJECT_SOURCE_DIR}/include)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加第三方库(add)头文件路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#生成静态库
add_library(${CURRENT_FOLDER} STATIC ${c_files})

#静态库的名字设置为当前目录名
set_target_properties(${CURRENT_FOLDER} PROPERTIES OUTPUT_NAME ${CURRENT_FOLDER})
