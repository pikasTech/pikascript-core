#include "test_mimiCore.h"
#include "test_solutions.h"
#include <iostream>
extern "C"
{
#include "dataMemory.h"
}

extern DMEM_STATE DMEMS;

void test()
{
    std::cout << "Test starting..." << std::endl
              << "================================" << std::endl;
    test_mimiCore(0);
    // the test_solutions should be void in core branch, should only used in solution branchs.
    // if the test case in test_solutions is nessciry to merge into test_mimiCore, merge it by hand please.
    test_solutions();
    std::cout << "Test of solutions Finished! " << std::endl
              << "Memory still used: " << DMEMS.blk_num << std::endl
              << "================================" << std::endl;
}

int main()
{
    test();
    return 0;
}
