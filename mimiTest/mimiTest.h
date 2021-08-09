#ifndef __MY_TEST_TOOL_H
#define __MY_TEST_TOOL_H
#include <stdint.h>
typedef int32_t (*test_fun_t)(int);

typedef int32_t (*assert_return_fun_t)(test_fun_t test_fun, int32_t isShow, char *test_name);

int assert_return(int (*test_fun)(int), int32_t isShow, char *test_name);

int detector_memoryCheck(assert_return_fun_t assert_result_fun, test_fun_t test_fun, int32_t isShow, char *test_name);

// #define ASSERT_RETURN(fun_name, input) detector_memoryCheck(assert_return, fun_name, isShow, (char *)#fun_name)

#define ASSERT_RETURN(fun_name, isShow) (assert_return(fun_name, isShow, (char *)#fun_name))

#endif