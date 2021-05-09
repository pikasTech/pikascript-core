#include "test_mimiCore.h"
#include "mimiTest.h"
#include <iostream>
extern "C"
{
// tools
#include "dataLink.h"
#include "mimiStr.h"
#include "mimiTest.h"
#include <string.h>
// cores
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "mimiVM_core.h"
// shApps
#include "shApp_argv.h"
#include "shApp_cmdNoFound.h"
#include "shApp_test.h"
// shDrivers
#include "shDriver_VM_iot.h"
#include "shDriver_sendTo.h"
#include "shDriver_uart.h"
// VM
#include "VM_delay.h"
#include "VM_iot.h"
// SubTest
#include "VM_com_test.h"
#include "VM_dht11_test.h"
#include "VM_memory.h"
#include "VM_socket_test.h"
#include "test_VM_key.h"
#include "test_dataLink.h"
#include "test_dataLog.h"
#include "test_dataQueue.h"
#include "test_PORT_strprint_linux.h"
}
extern DMEM_STATE DMEMS;
extern int error_num;
extern int memory_warnning_num;
int shell_test_tool(char *cmd, int isShow)
{
    char *cmdOut;
    DMEM *memOut;
    unsigned short int cmdOutSize;
    sh_t *sh = ShellCore_init();
    memOut = (DMEM *)(sh->cmd(sh, cmd));
    cmdOut = (char *)(memOut->addr);
    cmdOutSize = strGetSize(cmdOut);
    if (isShow)
    {
        std::cout << "cmdOut : " << std::endl
                  << "<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << cmdOut;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        std::cout << "The Size of cmdOut is : " << cmdOutSize << std::endl;
    }
    DynMemPut(memOut);
    sh->deinit(sh);
    return cmdOutSize;
}

int TEST_sendTo(int isShow)
{
    if (shell_test_tool((char *)"to com1 srting test", isShow) != 77)
    {
        return 1;
    }
    if (shell_test_tool((char *)"to com2 srting test 4iix", isShow) != 87)
    {
        return 2;
    }
    return 0;
}
int TEST_strGetArgs(int isShow)
{
    char cmd[] = "test the str get args";
    char *argv[16];
    DMEM *mem[16];
    // sign in memory to argv
    for (int i = 0; i < 16; i++)
    {
        mem[i] = DynMemGet(sizeof(char) * 16);
        argv[i] = (char *)(mem[i]->addr);
    }

    int argc;

    argc = strGetArgs(cmd, argv);
    if (isShow)
    {
        for (int i = 0; i < argc; i++)
        {
            std::cout << argv[i] << std::endl;
        }
    }
    if (strGetSize(argv[0]) != 4)
    {
        return 1;
    }
    if (strGetSize(argv[1]) != 3)
    {
        return 2;
    }
    if (strGetSize(argv[2]) != 3)
    {
        return 3;
    }
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(mem[i]);
    }
    return 0;
}

int TEST_app_argv_by_Shell(int isShow)
{
    if (shell_test_tool((char *)"argv testtt", isShow) != 48)
    {
        return 1;
    }
    if (shell_test_tool((char *)"argv testtt afe ckx 123j 'Iz -34", isShow) != 135)
    {
        return 2;
    }
    return 0;
}
int TEST_app_argv(int isShow)
{
    int argc = 4;
    char *argv[] = {(char *)"a", (char *)"bcd", (char *)"efg", (char *)"hig"};
    char *Strout;
    DMEM *memOut;

    memOut = (DMEM *)app_argv_main(argc, argv);
    Strout = (char *)(memOut->addr);
    int strOutSize = strGetSize(Strout);
    if (isShow)
    {
        std::cout << "app test output: " << std::endl
                  << "<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << Strout << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        std::cout << "The Size of cmdOut is : " << strOutSize << std::endl;
    }
    if (strOutSize != 50)
    {
        return 1;
    }
    DynMemPut(memOut);
    return 0;
}
int TEST_BaseShellFunction(int isShow)
{
    unsigned short int cmdOutSize;
    char cmd_1[] = "test testtt";
    char cmd_2[] = "test open";
    char cmd_3[] = "te";
    char cmd_4[] = "";
    sh_t *sh = ShellCore_init();

    DMEM *memOut;
    for (int i = 0; i < 4; i++)
    {
        char *cmdOut;
        if (i == 0)
        {
            memOut = (DMEM *)sh->cmd(sh, cmd_1);
            cmdOut = (char *)memOut->addr;
        }
        if (i == 1)
        {
            memOut = (DMEM *)sh->cmd(sh, cmd_2);
            cmdOut = (char *)memOut->addr;
        }
        if (i == 2)
        {
            memOut = (DMEM *)sh->cmd(sh, cmd_3);
            cmdOut = (char *)memOut->addr;
        }
        if (i == 3)
        {
            for (int j = 0; j < 200; j++)
            {
                memOut = (DMEM *)sh->cmd(sh, cmd_4);
                DynMemPut(memOut);
            }
            memOut = (DMEM *)sh->cmd(sh, cmd_4);
            cmdOut = (char *)memOut->addr;
        }
        cmdOutSize = strGetSize(cmdOut);
        if (isShow)
        {
            std::cout << "cmdOut : " << std::endl
                      << "<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << cmdOut;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            std::cout << "The Size of cmdOut is : " << cmdOutSize << std::endl;
        }
        if (i == 0)
        {
            if (cmdOutSize != 130)
            {
                return 1;
            }
        }
        if (i == 1)
        {
            if (cmdOutSize != 82)
            {
                return 2;
            }
        }
        if (i == 2)
        {
            if (cmdOutSize != 55)
            {
                return 3;
            }
        }
        DynMemPut(memOut);
    }
    sh->deinit(sh);
    return 0;
}

int VM_simulator_main(int argc, char **argv)
{
    PubSh = ShellCore_init();
    int isShow = *(argv[2]);
    int expect_log_size = *((int *)(argv[3]));
    DynMemPut((DMEM *)PubSh->cmd(PubSh, (char *)"iot init"));
    DMEM *memOut;
    memOut = (DMEM *)(PubSh->cmd(PubSh, (char *)"uart init"));
    DynMemPut(memOut);
    if ((strcmp(argv[0], "com1") == 0) || (strcmp(argv[0], "com2") == 0))
    {
        struct com_t *pcom;
        int VM_IT_name;
        if (0 == strcmp(argv[0], (char *)"com1"))
        {
            pcom = &VM.com1;
            VM_IT_name = VM_IT_COM1;
        }
        if (0 == strcmp(argv[0], (char *)"com2"))
        {
            pcom = &VM.com2;
            VM_IT_name = VM_IT_COM2;
        }
        char *VM_com_input = argv[1];
        for (int i = 0; i < 1000; i++)
        {
            // simulate the VM_com data input with IT.
            if (i < strGetSize(VM_com_input))
            {
                *VM_com_RX_Char_pget(pcom) = VM_com_input[i];
                VM_IT(VM_IT_name);
            }
            VM_vIT();
            VM_main();
        }
    }
    if (isShow)
    {
        printf("%s", VM.com1.loger->log);
        printf("%s", VM.com2.loger->log);
        printf("the log size of test is: %d\r\n", VM.com1.loger->log_size + VM.com2.loger->log_size);
    }
    if (expect_log_size != (VM.com1.loger->log_size + VM.com2.loger->log_size))
    {
        return 1;
    }
    DynMemPut((DMEM *)PubSh->cmd(PubSh, (char *)"iot deinit"));
    VM.com1.loger->deinit(VM.com1.loger);
    VM.com2.loger->deinit(VM.com2.loger);
    PubSh->deinit(PubSh);
    return 0;
}

int TEST_singleLine_callBack(int isShow)
{
    // char * argv[] =  {"com1", "test ttfefe \r\ntest i\r\nargv fjeiow if io - \r\nto com1 tetr\r\nto com2 fiii\r\n"};

    // char * argv[] =  {"com2", "test ttfefe \r\ntest i\r\nargv fjeiow if io - \r\nto com1 tetr\r\nto com2 fiii\r\n"};

    // char *argv[] = {"com1", "to com2 test jjt\r\n"};

    // char *argv[] = {"com1", "iot init\r\niot set tem 200\r\n"};

    // char *argv[] = {"com2", "8,01050000FF008C3A\r\n"};
    // char *argv[] = {"com1", "iot read\r\n"};

    int expect_log_size = 90;
    char isShow_char = isShow;

    char *argv[] = {(char *)"com1", (char *)"iot set bit 255\r\n", &isShow_char, (char *)(&expect_log_size)};

    return VM_simulator_main(4, argv);
}

int TEST_iot_shell(int isShow)
{
    char isShow_char = isShow;
    int expect_log_size = 364;
    char *argv[] = {(char *)"com1", (char *)"iot connect\r\n", &isShow_char, (char *)(&expect_log_size)};
    return VM_simulator_main(4, argv);
}

int TEST_VM_iot(int isShow)
{
    struct VM_iot *iot = VM_iot_Init();
    int testOut = 0;
    testOut = iot->test(iot, isShow);
    iot->DeInit(iot);
    return testOut;
}

void test_mimiCore(int isShow)
{
    // ASSERT_RETURN(TEST_VM_iot, isShow);
    // ASSERT_RETURN(TEST_iot_shell, isShow);
    // ASSERT_RETURN(TEST_singleLine_callBack, isShow);

    ASSERT_RETURN(TEST_dataLink, isShow);
    ASSERT_RETURN(TEST_app_argv_by_Shell, isShow);
    ASSERT_RETURN(TEST_BaseShellFunction, isShow);
    ASSERT_RETURN(TEST_app_argv, isShow);
    ASSERT_RETURN(TEST_strGetArgs, isShow);
    ASSERT_RETURN(TEST_sendTo, isShow);
    ASSERT_RETURN(TEST_VM_com_test_list, isShow);
    ASSERT_RETURN(TEST_VM_dht11, isShow);
    ASSERT_RETURN(TEST_VM_socket, isShow);
    ASSERT_RETURN(TEST_datalog, isShow);
    // ASSERT_RETURN(TEST_dataQueue, 1);
    ASSERT_RETURN(TEST_VM_key, isShow);
    ASSERT_RETURN(TEST_PORT_strprint_linux, isShow);
    std::cout << "Test of mimiCore Finished! " << std::endl
              << std::endl
              << "Memory still used: " << DMEMS.blk_num << std::endl
              << "There is " << error_num << " error(s)" << std::endl
              << "There is " << memory_warnning_num << " memory warnning(s)" << std::endl
              << "================================" << std::endl;
}