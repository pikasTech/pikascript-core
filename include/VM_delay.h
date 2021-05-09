#ifndef __VM_DELAY_H
#define __VM_DELAY_H

#include "dataMemory.h"
struct VM_delay
{
    void (*delay)(struct VM_delay *, int);
    DMEM *mem;
    void (*deinit)(struct VM_delay *);
    void *context;
};

struct VM_delay *VM_delay_init(void);

extern struct VM_delay *Pub_VM_Delay;

#endif