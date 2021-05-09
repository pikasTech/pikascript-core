#ifndef _VM_args__H
#define _VM_args__H
#include "VM_memory.h"
typedef struct Class_args args_t;
struct Class_args
{
    /* attribute */
    DMEM *mem;
    void *context;
    int count;
    DMEM *list_mem[32];
    char *list[32];

    /* operation */
    void (*dinit)(args_t *self);
    void (*init)(args_t *self, int argc, char **argv);
    int (*get)(args_t *self, int index, char **arg_get);
    float (*get_float)(args_t *self, int index);
    int (*put)(args_t *self, char *arg_put);
    int (*put_float)(args_t *self, float arg_float);
    

    /* virtual operation */

    /* object */
};

args_t *New_args(int argc, char **argv);
#endif