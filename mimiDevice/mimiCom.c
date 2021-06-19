#include "mimiCom.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(mimiCom_t *self)
{
    DynMemPut(self->mem);
    self->args->dinit(self->args);
}

static void getChar(mimiCom_t *self, char inputChar)
{
    int RxBuff_i = (int)self->args->getInt(self->args, "RxBuff_i");
    char *RxBuffer = NULL;
    self->args->getStr(self->args, "RxBuff", &RxBuffer);

    // return if there is too much rx
    if (!(RxBuff_i < 256))
    {
        return;
    }

    if (inputChar != '\r' && inputChar != '\n')
    {
        strPrintWithSize(RxBuffer, &inputChar, 1, 0);

        RxBuff_i++;

        self->args->setStr(self->args, "RxBuff", RxBuffer);
        self->args->setInt(self->args, "RxBuff_i", RxBuff_i);
    }
    if (inputChar == '\r')
    {
        self->args->setStr(self->args, "RxSingleLine", RxBuffer);
    }
}

static void init(mimiCom_t *self, args_t *initArgs)
{
    /* attrivute */
    self->context = self;
    self->args = New_args(NULL);

    self->args->setInt(self->args, "RxBuff_i", 0);
    self->args->setStr(self->args, "RxBuff", "");
    self->args->setStr(self->args, "RxSingleLine", "");
    self->args->setStr(self->args, "name", "com");

    /* operation */
    self->dinit = deinit;
    self->getChar = getChar;

    /* object */

    /* override */
}

mimiCom_t *New_mimiCom(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(mimiCom_t));
    mimiCom_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}