#ifndef __MY_TEST_TOOL_H
#define __MY_TEST_TOOL_H

typedef int (*test_fun_t)(int);

typedef int (*assert_return_fun_t)(test_fun_t test_fun, int isShow, char *test_name);

int assert_return(int (*test_fun)(int), int isShow, char *test_name);

int detector_memoryCheck(assert_return_fun_t assert_result_fun, test_fun_t test_fun, int isShow, char *test_name);

// #define ASSERT_RETURN(fun_name, input) detector_memoryCheck(assert_return, fun_name, isShow, (char *)#fun_name)

#define ASSERT_RETURN(fun_name, isShow) (assert_return(fun_name, isShow, (char *)#fun_name))

#endif