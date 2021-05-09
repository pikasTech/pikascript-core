#ifndef _inductor__H
#define _inductor__H
#include "VM_memory.h"
typedef struct Class_inductor inductor_t;
struct Class_inductor
{
    DMEM *mem;
    void (*dinit)(inductor_t *inductor);
    void (*update)(inductor_t *inductor, int systeme);
    int (*PORT_stateUpdate)(void);
    int state;
    int enable;

    void (*open_callback)(inductor_t *inductor);
    void (*close_callback)(inductor_t *inductor);

    void *context;

    int time_remain;
    int time_opened;
};

inductor_t *Class_inductor_NEW(void);
#endif