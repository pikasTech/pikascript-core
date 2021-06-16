#ifndef _dataList__H
#define _dataList__H
#include "dataLink.h"
#include "dataList.h"
#include "dataMemory.h"

typedef struct Class_Arglist list_t;
struct Class_Arglist
{
    /* attribute */
    DMEM *mem;
    void *context;
    link_t *argLinkList;
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    unsigned char nameBuff[ARG_NAME_LENGTH];

    /* operation */
    void (*dinit)(list_t *self);
    void (*init)(list_t *self, list_t *args);
    int (*size)(list_t *self);
    int (*pushContant)(list_t *self, char *type);
    int (*getIndexByName)(list_t *self, char *name);

    arg_t *(*getArgByIndex)(list_t *self, int index);

    char *(*getTypeByName)(list_t *self, char *name);

    int (*pushStrWithName)(list_t *self, char *name, char *strIn);
    int (*getStrByName)(list_t *self, char *name, char **strOut);

    int (*pushStrWithDefaultName)(list_t *self, char *strIn);
    int (*getStrByIndex)(list_t *self, int index, char **strOut);

    int (*pushFloatWithDefaultName)(list_t *self, float argFloat);
    int (*pushFloatWithName)(list_t *self, char *name, float argFloat);
    float (*getFloatByIndex)(list_t *self, int index);

    int (*pushPointerWithName)(list_t *self, char *name, void *argPointer);
    void *(*getPointerByName)(list_t *self, char *name);
    void *(*getPointerByIndex)(list_t *self, int index);

    int (*pushInt64WithName)(list_t *self, char *name, long long int64In);
    long long (*getInt64ByName)(list_t *self, char *name);
    long long (*getInt64ByIndex)(list_t *self, int index);

    /* virtual operation */

    /* object */
};

list_t *New_list(list_t *args);
#endif