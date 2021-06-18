#ifndef _Process__H
#define _Process__H
#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_process process_t;

struct Class_process
{
    /* attribute */
    DMEM *mem;

    /* list */
    link_t *subServerList;
    args_t *attributeList;

    /* operation */
    void (*dinit)(process_t *self);
    void (*init)(process_t *self, args_t *args);
    void (*update)(process_t *self, int sysytime);
    void (*enable)(process_t *self);
    void (*disable)(process_t *self);

    void (*setInt64)(process_t *self, char *name, long long val);
    long long (*getInt64)(process_t *self, char *name);

    void (*setPointer)(process_t *self, char *name, void *pointer);
    void *(*getPointer)(process_t *self, char *name);

    void (*setFloat)(process_t *self, char *name, float value);
    float (*getFloat)(process_t *self, char *name);

    void (*setStr)(process_t *self, char *name, char *str);
    void (*getStr)(process_t *self, char *name, char **strOut);

    void (*loadAttributeFromArgs)(process_t *self, args_t *args, char *name);

    /* virtual operation */

    /* object */
};

process_t *New_server2(args_t *args);
#endif
