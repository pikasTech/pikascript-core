#ifndef __MIMI_SHELL2_H
#define __MIMI_SHELL2_H
#include "dataMemory.h"
#include "dataArgs.h"

typedef struct Class_mimiShell2 Shell;
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
    void *(*cmd)(Shell *self, char *);
    void (*addMap)(Shell *self, char *, void *(*)(Shell *shell, int argc, char **argv));
    int (*listMap)(Shell *self, int);
    int (*test)(Shell *self, int);
    void (*init)(Shell *self, Args *args);
    void (*deinit)(Shell *self);

    /* override */
    void (*config)(Shell *self);
    void *(*detector)(Shell *self, void *(*fun_d)(Shell *self, char *, void *(fun)(Shell *self, int, char **)),
                      char *CMD,
                      void *(fun)(Shell *self, int argc, char **argv));
};

Shell *New_shell(Args *initArgs);

#endif
