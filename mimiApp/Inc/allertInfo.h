#ifndef _allertInfo__H
#define _allertInfo__H
#include "dataList.h"
#include "dataMemory.h"
typedef struct Class_allertInfo allertInfo_t;
struct Class_allertInfo
{
    /* attribute */
    DMEM *mem;
    void *context;
    int allert_temp1;
    int allert_temp2;

    /* operation */
    void (*dinit)(allertInfo_t *self);
    void (*init)(allertInfo_t *self, list_t *args);

    /* virtual operation */

    /* object */
};

allertInfo_t *New_allertInfo(list_t *args);
#endif