#ifndef _mimiCom__H
#define _mimiCom__H
#include "dataMemory.h"
#include "dataArgs.h"
typedef struct Class_mimiCom MimiCom;
struct Class_mimiCom
{
    /* attribute */
    DMEM *mem;
    Args *args;
    char RxBuff[256];

    /* operation */
    void (*deinit)(MimiCom *self);
    void (*init)(MimiCom *self, Args *args);

    void (*getChar)(MimiCom *self, char inputChar);
    void (*sendSting)(MimiCom *self, char *stingToSend);

    /* virtual operation */
    void (*_singleLineCallBack)(MimiCom *self);
    void (*_sendStringHandle)(MimiCom *self, char *stingToSend);

    /* object */
};

MimiCom *New_mimiCom(Args *args);
#endif
