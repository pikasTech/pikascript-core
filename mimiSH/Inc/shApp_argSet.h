#ifndef __APP_ARG_SET_H
#define __APP_ARG_SET_H
#include "mimiProcess.h"
#include "mimiShell2.h"
void *app_argSet(shell2_t *shell, int argc, char **argv);
mimiProcess_t *goToProcess(mimiProcess_t *root, char *processDirectory, int deepth);
#endif
