#ifndef __APP_ARG_SET_H
#define __APP_ARG_SET_H
#include "MimiProcess.h"
#include "mimiShell.h"
void *app_argSet(Shell *shell, int argc, char **argv);
MimiProcess *goToProcess(MimiProcess *root, char *processDirectory, int deepth);
#endif
