#include "VM_keyBoard_4key.h"
#include "VM_key.h"
#include "VM_memory.h"

static void deinit(keyBoard4_t *kb)
{
    DynMemPut(kb->mem);
}

keyBoard4_t *VM_keyBoard_4Key_init(void)
{
    DMEM *mem = DynMemGet(sizeof(keyBoard4_t));
    keyBoard4_t *kb = (keyBoard4_t *)mem->addr;
    kb->mem = mem;
    kb->deinit = deinit;
    return kb;
}