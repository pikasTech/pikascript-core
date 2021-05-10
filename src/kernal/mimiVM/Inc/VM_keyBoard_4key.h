#ifndef __VM__KEYBOARD_4KEY__H
#define __VM__KEYBOARD_4KEY__H
#include "dataMemory.h"
#include "VM_key.h"
typedef struct VM_keyBoard_4key keyBoard4_t;
struct VM_keyBoard_4key
{
    DMEM *mem;
    void (*deinit)(keyBoard4_t *kb);
    void (*update)(keyBoard4_t *kb);
    vkey_t *key_up;
    vkey_t *key_down;
    vkey_t *key_enter;
    vkey_t *key_back;
};

keyBoard4_t *VM_keyBoard_4Key_init(void);

#endif