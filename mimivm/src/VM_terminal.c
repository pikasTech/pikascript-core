#include "VM_terminal.h"
#include "VM_memory.h"
static void deinit(terminal_t *terminal)
{
    DynMemPut(terminal->mem);
}

static char PORT_VM_terminal_getChar(void)
{
    //rewrite it in port file.
    return 0;
}

static void PORT_VM_terminal_exit(void)
{
    //rewrite it in port file. 
}

static void callBack_char(terminal_t *terminal, char ch)
{
    //rewrite it in user code
    if (3 == ch)
    {
        terminal->PORT_VM_terminal_exit();
    }
}

static void update(terminal_t *terminal)
{
    char ch = terminal->PORT_VM_terminal_getChar();
    if (ch)
    {
        terminal->callBack_char(terminal, ch);
    }
}

terminal_t *VM_terminal_init(void)
{
    DMEM *mem = DynMemGet(sizeof(terminal_t));
    terminal_t *terminal = mem->addr;
    terminal->dinit = deinit;
    terminal->callBack_char = callBack_char;
    terminal->PORT_VM_terminal_getChar = PORT_VM_terminal_getChar;
    terminal->update = update;
    terminal->isClearLastLine = 0;
    terminal->PORT_VM_terminal_exit = PORT_VM_terminal_exit;

    return terminal;
}