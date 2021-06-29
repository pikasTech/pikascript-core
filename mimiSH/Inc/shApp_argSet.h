#ifndef __APP_ARG_SET_H
#define __APP_ARG_SET_H
#include "mimiProcess.h"
void *app_argSet(int argc, char **argv);
mimiProcess_t *goToProcess(mimiProcess_t *root, char *processDirectory, int deepth);
#endif
