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
    link_t *subOjbectLinkList;
    link_t *subAttributeLinkList;
    void *subObjectList[SERVER_SUB_OBJECT_LIST_LENGTH];
    void *subAttributeList[SERVER_SUB_ATTRIBUTE_LIST_LENGTH];

    /* operation */
    void (*dinit)(server_t *self);
    void (*init)(server_t *self, args_t *args);
    void (*update)(server_t *self, int sysytime);
    void (*enable)(server_t *self);
    void (*disable)(server_t *self);
    void (*argHandle)(server_t *self,
                      args_t *args,
                      char *argName,
                      void (*handle)(server_t *self,
                                     args_t *args,
                                     char *argName));

    /* virtual operation */

    /* object */
};

server_t *New_server(args_t *args);
#endif
