#ifndef __SHELL_CORE_H
#define __SHELL_CORE_H
#include "dataMemory.h"
#include "dataLinkWithNode.h"
char *ShellCMDPush(char *CMD);
int strGetArgs(char *CMD, char **argv);

typedef struct cmdMap
{
    DMEM *mem;
    char *cmdName;
    void *(*cmdCallBack)(int, char **);
} cmdMap_t;

typedef struct ShellCore
{
    DMEM *mem;
    void *(*cmd)(struct ShellCore *, char *);
    linkWithNode_t *cmdMapHead;
    void (*addMap)(struct ShellCore *, char *, void *(*)(int argc, char **argv));
    int (*listMap)(struct ShellCore *, int);
    int (*test)(struct ShellCore *, int);
    void (*config)(struct ShellCore *);
    void (*deinit)(struct ShellCore *);
    void *(*detector)(void *(*fun_d)(char *, char *, void *(fun)(int, char **)), char *CMD, char *StartStr, void *(fun)(int argc, char **argv));
} sh_t;

sh_t *ShellCore_init(void);

#endif
