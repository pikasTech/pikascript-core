#ifndef _goods__H
#define _goods__H
#include "dataMemory.h"
typedef struct Class_goods goods_t;
struct Class_goods
{
    DMEM *mem;
    void (*dinit)(goods_t *goods);

    char name[32];
    int price;
    int buy_number;

    void (*setName)(goods_t *goods,char * Name);
    void *feild;
};
goods_t *Class_goods_init(void);
#endif