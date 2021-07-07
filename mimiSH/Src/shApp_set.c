#include "shApp_set.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "server.h"
#include "tempInfo.h"
#include <stdlib.h>

extern server_t *tempM;
static void *sh_main(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    char *strOut = (char *)(memOut->addr);
    strOut[0] = 0;

    tempInfo_t *tempInfo = tempM->subObjectList[0];
    tempInfo->set(tempInfo, argv[0], atoi(argv[1]));

    sprintf(strOut, "set var to %d\r\n", atoi(argv[1]));

    return (void *)memOut;
}

static void deinit(shApp_set_t *self)
{
    DynMemPut(self->mem);
}

static void init(shApp_set_t *self, Args *args)
{
    /* attrivute */
    self->context = self;
    self->sh_main = sh_main;

    /* operation */
    self->deinit = deinit;

    /* object */

    /* override */
}

shApp_set_t *New_shApp_set(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(shApp_set_t));
    shApp_set_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}