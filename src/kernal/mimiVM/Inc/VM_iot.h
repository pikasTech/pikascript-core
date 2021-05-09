#ifndef __VM_IOT_H
#define __VM_IOT_H

typedef struct VM_iot iot_t;
#include "VM_memory.h"

struct VM_iot
{
    DMEM *mem;
    void (*send)(struct VM_iot *, char *);
    void (*info)(struct VM_iot *, char *);
    void (*DeInit)(struct VM_iot *);
    int (*test)(struct VM_iot *, int);
    int (*connect)(struct VM_iot *, int);
    void (*delay)(struct VM_iot *, int);
    void (*upload)(iot_t *, char *);
    void (*data_upload)(iot_t *, unsigned char *, unsigned char *);

    void (*data_upload_int)(iot_t *iot, int addr, int data);

    void (*bit_upload)(iot_t *, unsigned char *, unsigned char *);
    // the call back of VM_iot
    void (*callBack)(iot_t *, void *);
    void (*readBuff)(iot_t *);
    int (*get_value_function_06)(iot_t *, char *strGet);
    int (*get_addr_function_06)(iot_t *, char *strGet);
    struct VM_delay *vd;
    void *context;
};

struct VM_iot *VM_iot_Init(void);

#endif