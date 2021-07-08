#ifndef __APP_ARG_SET_H
#define __APP_ARG_SET_H
#include "mimiProcess.h"
#include "mimiShell.h"
void *app_argSet(Shell2 *shell, int argc, char **argv);
MimiProcess *goToProcess(MimiProcess *root, char *processDirectory, int deepth);
#endif
