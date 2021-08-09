#include "test_mimiCore.h"
#include "mimiTest.h"
#include <iostream>
extern "C"
{
// tools
#include "dataString.h"
#include "mimiTest.h"
#include <string.h>
// cores
#include "mimiShell.h"
// SubTest
#include "dataMemory.h"
#include "test_args.h"
#include "test_dataLog.h"
#include "test_link.h"
#include "test_MimiObj.h"
#include "test_event.h"
#include "test_sysObj.h"
// Device
#include "test_mimiCom.h"
}
extern DMEM_STATE DMEMS;
extern int error_num;
extern int memory_warnning_num;

void test_mimiCore(int isShow)
{
    ASSERT_RETURN(TEST_datalog, isShow);
    ASSERT_RETURN(TEST_link2, isShow);
    ASSERT_RETURN(TEST_args2, isShow);
    ASSERT_RETURN(TEST_mimiCom, isShow);
    ASSERT_RETURN(TEST_Event, isShow);
    ASSERT_RETURN(TEST_MimiObj, 1);
    ASSERT_RETURN(TEST_sysObj, 1);
    std::cout << "Test of mimiCore Finished! " << std::endl
              << std::endl
              << "Memory still used: " << (float)DMEMS.blk_num * 32 / 1024 << " kB" << std::endl
              << "There is " << error_num << " error(s)" << std::endl
              << "There is " << memory_warnning_num << " memory warnning(s)" << std::endl
              << "================================" << std::endl;
}
