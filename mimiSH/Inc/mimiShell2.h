#ifndef __MIMI_SHELL2_H
#define __MIMI_SHELL2_H
#include "dataMemory.h"
#include "dataArgs.h"

typedef struct Class_mimiShell2 Shell2;
#define SHELL2_CMD_NAME_LENGTH 32
#define SHELL2_CMD_LENGTH 256

int mimiShell2_strGetArgs(char *CMD, char **argv);

struct Class_mimiShell2
{
    /* attribute */
    DMEM *mem;
    Args *mapList;

    void *context;

    /* operation */
    void *(*cmd)(Shell2 *self, char *);
    void (*addMap)(Shell2 *self, char *, void *(*)(Shell2 *shell, int argc, char **argv));
    int (*listMap)(Shell2 *self, int);
    int (*test)(Shell2 *self, int);
    void (*init)(Shell2 *self, Args *args);
    void (*deinit)(Shell2 *self);

    /* override */
    void (*config)(Shell2 *self);
    void *(*detector)(Shell2 *self, void *(*fun_d)(char *, void *(fun)(int, char **)),
                      char *CMD,
                      void *(fun)(int argc, char **argv));
};

Shell2 *New_shell2(Args *initArgs);

#endif
