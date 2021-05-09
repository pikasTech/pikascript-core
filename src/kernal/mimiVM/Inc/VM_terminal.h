#ifndef __VM_terminal__H
#define __VM_terminal__H
#include "dataMemory.h"
typedef struct VM_terminal terminal_t;
struct VM_terminal
{
    DMEM *mem;
    void (*dinit)(terminal_t *terminal);

    void (*callBack_char)(terminal_t *terminal, char ch);
    void (*update)(terminal_t *terminal);
    void (*PORT_VM_terminal_exit)(void);
    char (*PORT_VM_terminal_getChar)(void);
    int isClearLastLine;
};

terminal_t *VM_terminal_init(void);
#endif