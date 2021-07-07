#ifndef _server__H
#define _server__H
#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_server server_t;

#define SERVER_SUB_OBJECT_LIST_LENGTH 8
#define SERVER_SUB_ATTRIBUTE_LIST_LENGTH 16

struct Class_server
{
    /* attribute */
    DMEM *mem;
    void *context;
    int isEnable;
    Link *subOjbectLinkList;
    Link *subAttributeLinkList;
    void *subObjectList[SERVER_SUB_OBJECT_LIST_LENGTH];
    void *subAttributeList[SERVER_SUB_ATTRIBUTE_LIST_LENGTH];

    /* operation */
    void (*deinit)(server_t *self);
    void (*init)(server_t *self, Args *args);
    void (*update)(server_t *self, int sysytime);
    void (*enable)(server_t *self);
    void (*disable)(server_t *self);
    void (*argHandle)(server_t *self,
                      Args *args,
                      char *argName,
                      void (*handle)(server_t *self,
                                     Args *args,
                                     char *argName));

    /* virtual operation */

    /* object */
};

server_t *New_server(Args *args);
#endif
