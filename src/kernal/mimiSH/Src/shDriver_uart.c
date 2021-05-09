#include "dataMemory.h"
#include "mimiSH_core.h"
#include "mimiVM_core.h"
#include <stdlib.h>
#include <string.h>
void *sDriver_Uart(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    // char *strOut = (char *)(memOut->addr);
    if (strcmp(argv[0], "init") == 0)
    {
        VMdriver_Uart_init();
        VM_UART_Transmit(&VM.com1, "uart init ok!\r\n");
    }
    return (void *)memOut;
}
