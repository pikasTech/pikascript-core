#ifndef _server2__H
#define _server2__H
#include "dataList.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_server2 server2_t;

struct Class_server2
{
    /* attribute */
    DMEM *mem;

    /* list */
    link_t *subServerList;
    list_t *attributeList;

    /* operation */
    void (*dinit)(server2_t *self);
    void (*init)(server2_t *self, list_t *args);
    void (*update)(server2_t *self, int sysytime);
    void (*enable)(server2_t *self);
    void (*disable)(server2_t *self);

    void (*setInt64)(server2_t *self, char *name, long long val);
    long long (*getInt64)(server2_t *self, char *name);

    void (*setPointer)(server2_t *self, char *name, void *pointer);
    void *(*getPointer)(server2_t *self, char *name);

    void (*setFloat)(server2_t *self, char *name, float value);
    float (*getFloat)(server2_t *self, char *name);

    void (*setStr)(server2_t *self, char *name, char *str);
    void (*getStr)(server2_t *self, char *name, char **strOut);

    void (*loadAttributeFromArgs)(server2_t *self, list_t *args, char *name);

    /* virtual operation */

    /* object */
};

server2_t *New_server2(list_t *args);
#endif
