#设置 BINARY 为项目名IndexProject
set(BINARY ${CMAKE_PROJECT_NAME})
 
# 1
# add_executable(ExampleProject main.cpp file1.cpp file1.h)
 
# 2
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.c *.cpp)
set(SOURCES ${SOURCES})

add_library(${BINARY}_mimilib STATIC ${SOURCES})