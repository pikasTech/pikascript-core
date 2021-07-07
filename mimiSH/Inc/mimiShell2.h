#ifndef __MIMI_SHELL2_H
#define __MIMI_SHELL2_H
#include "dataMemory.h"
#include "dataLinkWithNode.h"
#include "dataArgs.h"

typedef struct Class_mimiShell2 shell2_t;
#define SHELL2_CMD_NAME_LENGTH 32
#define SHELL2_CMD_LENGTH 256

int mimiShell2_strGetArgs(char *CMD, char **argv);

typedef struct mimiShell2_cmdMap
{
    DMEM *mem;
    char *cmdName;
    void *(*cmdCallBack)(int, char **);
} mimiShell2_cmdMap_t;

struct Class_mimiShell2
{
    /* attribute */
    DMEM *mem;
    linkWithNode_t *cmdMapHead;
    void * context;

    /* operation */
    void *(*cmd)(shell2_t *self, char *);
    void (*addMap)(shell2_t *self, char *, void *(*)(shell2_t *shell, int argc, char **argv));
    int (*listMap)(shell2_t *self, int);
    int (*test)(shell2_t *self, int);
    void (*init)(shell2_t *self, args_t *args);
    void (*dinit)(shell2_t *self);

    /* override */
    void (*config)(shell2_t *self);
    void *(*detector)(shell2_t *self, void *(*fun_d)(char *, void *(fun)(int, char **)),
                      char *CMD,
                      void *(fun)(int argc, char **argv));
};

shell2_t *New_shell2(args_t *initArgs);

#endif
