#include "mimiCom.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(MimiCom *self)
{
    DynMemPut(self->mem);
    args_deinit(self->args);
}

static void singleLineCallBackDefault(MimiCom *self)
{
    /* overRide it in user code */
}

static void getChar(MimiCom *self, char inputChar)
{
    if (inputChar != '\r' && inputChar != '\n')
    {
        strAppendWithSize(self->RxBuff, &inputChar, 1);
    }
    if (inputChar == '\r')
    {
        args_setStr(self->args, "RxSingleLine", self->RxBuff);
        self->RxBuff[0] = 0;
        self->_singleLineCallBack(self);
    }
}

static void _sendStringHandleDefault(MimiCom *self, char *stringToSend)
{
    /* override it in user code */
}

static void sendSting(MimiCom *self, char *stingToSend)
{
    self->_sendStringHandle(self, stingToSend);
}

static void init(MimiCom *self, Args *initArgs)
{
    /* attribute */
    self->args = New_args(NULL);
    self->RxBuff[0] = 0;

    args_setPtr(self->args, "__context", self);
    args_setStr(self->args, "RxSingleLine", "");

    /* operation */
    self->deinit = deinit;
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
    args_copyArgByName(initArgs, "__context", self->args);
}

MimiCom *New_mimiCom(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiCom));
    MimiCom *self = (void *)(mem->addr);
    self->mem = mem;
    init(self, args);
    return self;
}
