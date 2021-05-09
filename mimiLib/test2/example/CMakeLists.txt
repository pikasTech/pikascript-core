# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加第三方库(add)头文件路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 生成执行文件
add_executable(test_add test_add.c)
add_executable(test_sub test_sub.c)

# 链接库文件
target_link_libraries(test_add add)
target_link_libraries(test_sub sub)

