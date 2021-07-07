#include "tempInfo.h"
#include "dataMemory.h"
#include <stdlib.h>

static void deinit(tempInfo_t *self)
{
    DynMemPut(self->mem);
}

static int get(tempInfo_t *self, char *varName)
{
    if (0 == strcmp(varName, "temp1"))
    {
        return self->temp1;
    }
    if (0 == strcmp(varName, "temp2"))
    {
        return self->temp2;
    }
    if (0 == strcmp(varName, "hum1"))
    {
        return self->hum1;
    }
    if (0 == strcmp(varName, "hum2"))
    {
        return self->hum2;
    }
		return 0;
}

static void set(tempInfo_t *self, char *varName, int var)
{
    if (0 == strcmp(varName, "temp1"))
    {
        self->temp1 = var;
        return;
    }
    if (0 == strcmp(varName, "temp2"))
    {
        self->temp2 = var;
        return;
    }
    if (0 == strcmp(varName, "hum1"))
    {
        self->hum1 = var;
    }
    if (0 == strcmp(varName, "hum2"))
    {
        self->hum2 = var;
    }
}

static void init(tempInfo_t *self, Args *args)
{
    /* attrivute */
    self->context = self;
    self->temp1 = 0;
    self->temp2 = 0;
    self->hum1 = 0;
    self->hum2 = 0;
		self->addr_temp1 = 0;
		self->addr_temp2 = 1;
		self->addr_hum1 = 2;
		self->addr_hum2 = 3;
	

    /* operation */
    self->deinit = deinit;
    self->set = set;
    self->get = get;

    /* object */

    /* override */
}

tempInfo_t *New_tempInfo(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(tempInfo_t));
    tempInfo_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}