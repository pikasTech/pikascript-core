#include "inductor.h"
#include "VM_memory.h"
static void deinit(inductor_t *inductor)
{
    DynMemPut(inductor->mem);
}

static int PORT_stateUpdate(void)
{
    //rewrite it in PORT file
    return 0;
}

static void open_callback(inductor_t *inductor)
{
    //rewrite it in user file
}

static void close_callback(inductor_t *inductor)
{
    //rewrite it in user file
}

static void update(inductor_t *idt, int systime)
{
    // return if not enable
    if (!idt->enable)
    {
        return;
    }

    idt->state = idt->PORT_stateUpdate();

    // return when remain still
    if ((systime - idt->time_opened) < idt->time_remain)
    {
        return;
    }

    if (!idt->state)
    {
        idt->close_callback(idt);
        return;
    }

    if (idt->state)
    {
        idt->open_callback(idt);
        idt->time_opened = systime;
        idt->time_remain = 3000;
        return;
    }
}

inductor_t *Class_inductor_NEW(void)
{
    DMEM *mem = DynMemGet(sizeof(inductor_t));
    inductor_t *inductor = mem->addr;

    inductor->open_callback = open_callback;
    inductor->close_callback = close_callback;
    inductor->state = 0;
    inductor->mem = mem;
    inductor->enable = 0;
    inductor->dinit = deinit;
    inductor->PORT_stateUpdate = PORT_stateUpdate;
    inductor->context = inductor;
    inductor->update = update;

    inductor->time_remain = 0;
    inductor->time_opened = 0;

    return inductor;
}