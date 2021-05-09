#ifndef __SH_sgui__H
#define __SH_sgui__H
#include "VM_memory.h"
typedef struct SH_sgui sgui_t;
struct SH_sgui
{
    DMEM *mem;
    void (*dinit)(sgui_t *sgui);
    void* (*sh_main)(int argc, char **argv);
};
sgui_t *SH_sgui_init(void);
#endif