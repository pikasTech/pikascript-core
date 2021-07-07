#ifndef _shApp_set__H
#define _shApp_set__H
#include "dataArgs.h"
#include "dataMemory.h"
typedef struct Class_shApp_set shApp_set_t;

struct Class_shApp_set
{
    /* attribute */
    DMEM *mem;
    void *context;
    void *(*sh_main)(int argc, char **argv);

    /* operation */
    void (*deinit)(shApp_set_t *self);
    void (*init)(shApp_set_t *self, Args *args);

    /* virtual operation */

    /* object */
};

shApp_set_t *New_shApp_set(Args *args);
#endif