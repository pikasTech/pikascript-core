#include "mimiTest.h"
#include <iostream>
extern "C"
{
#include "VM_memory.h"
}

extern DMEM_STATE DMEMS;

int error_num = 0;
int memory_warnning_num = 0;

int assert_return(test_fun_t test_fun, int isShow, char *test_name)
{
    // std::cout << "Testing: " << test_name << std::endl;
    int ret;
    int memory_used_befor = DMEMS.blk_num;
    ret = test_fun(isShow);
    int memory_used_after = DMEMS.blk_num;
    int memory_still_used = memory_used_after - memory_used_befor;
    if (0 != memory_still_used)
    {
        memory_warnning_num++;
        std::cout << "The memory still used is: " << memory_still_used << std::endl;
    }
    if (ret != 0)
    {
        error_num++;
        std::cout << "Testing: " << test_name << " error!"
                  << " return: " << ret << std::endl
                  << "-----------------------------" << std::endl;
        return ret;
    }

    std::cout << "Testing: " << test_name << " ok!" << std::endl
              << "-----------------------------" << std::endl;
    return 0;
}

int detector_memoryCheck(assert_return_fun_t assert_result_fun, test_fun_t test_fun, int isShow, char *test_name)
{
    int out = 0;
    out = assert_result_fun(test_fun, isShow, test_name);
    return out;
}