#ifndef _dataTest__H
#define _dataTest__H
#include "dataMemory.h"
#include "dataArgs.h"
typedef struct Class_dataTest dataTest_t;
struct Class_dataTest
{
    /* attribute */
    DMEM *mem;
    void *context;
    int a;
    int b;

    /* operation */
    void (*dinit)(dataTest_t *self);
    void (*init)(dataTest_t *self, args_t *args);

    /* virtual operation */

    /* object */
};

dataTest_t *New_dataTest(args_t *args);
#endif