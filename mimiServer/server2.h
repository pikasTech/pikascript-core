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
    void *context;
    int isEnable;
    link_t *subServerList;
    link_t *dataList;

    /* operation */
    void (*dinit)(server2_t *self);
    void (*init)(server2_t *self, list_t *args);
    void (*update)(server2_t *self, int sysytime);
    void (*enable)(server2_t *self);
    void (*disable)(server2_t *self);
    void (*argHandle)(server2_t *self,
                      list_t *args,
                      char *argName,
                      void (*handle)(server2_t *self,
                                     list_t *args,
                                     char *argName));

    /* virtual operation */

    /* object */
};

server2_t *New_server2(list_t *args);
#endif
