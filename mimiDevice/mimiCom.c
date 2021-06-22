#include "mimiCom.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(mimiCom_t *self)
{
    DynMemPut(self->mem);
    self->args->dinit(self->args);
}

static void singleLineCallBackDefault(mimiCom_t *self)
{
    /* overRide it in user code */
}

static void getChar(mimiCom_t *self, char inputChar)
{
    if (inputChar != '\r' && inputChar != '\n')
    {
        strPrintWithSize(self->RxBuff, &inputChar, 1);
    }
    if (inputChar == '\r')
    {
        self->args->setStr(self->args, "RxSingleLine", self->RxBuff);
        self->RxBuff[0] = 0;
        self->_singleLineCallBack(self);
    }
}

static void _sendStringHandleDefault(mimiCom_t *self, char *stringToSend)
{
    /* override it in user code */
}

static void sendSting(mimiCom_t *self, char *stingToSend)
{
    self->_sendStringHandle(self, stingToSend);
}

static void init(mimiCom_t *self, args_t *initArgs)
{
    /* attrivute */
    self->args = New_args(NULL);
    self->RxBuff[0] = 0;

    self->args->setPoi(self->args, "context", self);
    self->args->setStr(self->args, "RxSingleLine", "");

    /* operation */
    self->dinit = deinit;
    self->getChar = getChar;
    self->sendSting = sendSting;

    /* object */

    /* override */
    self->_singleLineCallBack = singleLineCallBackDefault;
    self->_sendStringHandle = _sendStringHandleDefault;

    /* load args */
    if (NULL == initArgs)
    {
        return;
    }
    initArgs->copyArg(initArgs, "context", self->args);
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
