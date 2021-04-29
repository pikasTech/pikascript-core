#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "shApp_argv.h"
#include "shApp_cmdNoFound.h"
#include "shApp_test.h"
#include "shDriver_VM_iot.h"
#include "shDriver_sendTo.h"
#include "shDriver_uart.h"

// the public sh
sh_t *PubSh;

void sh_config(sh_t *sh)
{
    // make the link list for the cmd Map
    sh->addMap(sh, "test ", app_test);
    sh->addMap(sh, "argv ", app_argv_main);
    sh->addMap(sh, "to ", sDriver_sendTo_main);
    sh->addMap(sh, "uart ", sDriver_Uart);
    sh->addMap(sh, "iot ", sDriver_VM_iot_main);
}