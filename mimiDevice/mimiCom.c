#include "mimiCom.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(MimiCom *self)
{
    DynMemPut(self->mem);
    self->args->deinit(self->args);
}

static void singleLineCallBackDefault(MimiCom *self)
{
    /* overRide it in user code */
}

static void getChar(MimiCom *self, char inputChar)
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
    /* attrivute */
    self->args = New_args(NULL);
    self->RxBuff[0] = 0;

    self->args->setPtr(self->args, "context", self);
    self->args->setStr(self->args, "RxSingleLine", "");

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
    initArgs->copyArg(initArgs, "context", self->args);
}

MimiCom *New_mimiCom(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiCom));
    MimiCom *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
