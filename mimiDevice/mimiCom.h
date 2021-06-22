#ifndef _mimiCom__H
#define _mimiCom__H
#include "dataMemory.h"
#include "dataArgs.h"
typedef struct Class_mimiCom mimiCom_t;
struct Class_mimiCom
{
    /* attribute */
    DMEM *mem;
    args_t *args;
    char RxBuff[256];

    /* operation */
    void (*dinit)(mimiCom_t *self);
    void (*init)(mimiCom_t *self, args_t *args);

    void (*getChar)(mimiCom_t *self, char inputChar);
    void (*sendSting)(mimiCom_t *self, char *stingToSend);

    /* virtual operation */
    void (*_singleLineCallBack)(mimiCom_t *self);
    void (*_sendStringHandle)(mimiCom_t *self, char *stingToSend);

    /* object */
};

mimiCom_t *New_mimiCom(args_t *args);
#endif