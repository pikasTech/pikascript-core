#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

#define ARG_CONTANT_LENGTH 128
#define ARG_NAME_LENGTH 32
#define ARG_TYPE_LENGTH 32

typedef struct Class_arg arg_t;
struct Class_arg
{
    /* attribute */
    DMEM *mem;
    void *context;

    char contant[ARG_CONTANT_LENGTH];
    char name[ARG_NAME_LENGTH];
    char type[ARG_TYPE_LENGTH];

    /* operation */
    void (*dinit)(arg_t *self);
    void (*init)(arg_t *self, void *voidPointer);

    void (*setName)(arg_t *self, char *name);
    void (*setContant)(arg_t *self, char *contant);
    void (*setType)(arg_t *self, char *type);
    
    char *(*get)(arg_t *self);

    /* virtual operation */

    /* object */
};

arg_t *New_arg(void *voidPointer);
#endif