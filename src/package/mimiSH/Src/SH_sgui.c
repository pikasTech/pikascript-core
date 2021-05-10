#include "SH_sgui.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(sgui_t *sgui)
{
    DynMemPut(sgui->mem);
}

static void *sh_main(int argc, char **argv)
{
    DMEM *strout_mem = DynMemGet(sizeof(char) * 256);
    char *strOut = strout_mem->addr;
    strOut[0] = 0;
    strPrint(strOut, "Running : sgui \r\n");
    
    return (void *)strout_mem;
}

sgui_t *SH_sgui_init(void)
{
    DMEM *mem = DynMemGet(sizeof(sgui_t));
    sgui_t *sgui = mem->addr;
    sgui->mem = mem;
    sgui->dinit = deinit;
    sgui->sh_main = sh_main;
    return sgui;
}