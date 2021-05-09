#递归获取目录下所有的C文件
file(GLOB_RECURSE c_files ./*.c)

# 递归获取目录下所有的h文件
file(GLOB_RECURSE h_files ./*.h)

#生成静态库
add_library(sub_lib_static STATIC ${c_files})

#将动态库和静态库的名字设置为 sub
set_target_properties(sub_lib_static PROPERTIES OUTPUT_NAME "sub")

file(COPY ${h_files}
DESTINATION ${PROJECT_SOURCE_DIR}/build/include)
