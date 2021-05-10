#include "goods.h"
#include "dataMemory.h"
#include "dataString.h"
static void deinit(goods_t *goods)
{
    DynMemPut(goods->mem);
}

static void setName(goods_t *goods, char *Name)
{
    for (int i = 0; i < 32; i++)
    {
        goods->name[i] = 0;
    }
    strPrint(goods->name, Name);
}

goods_t *Class_goods_init(void)
{
    DMEM *mem = DynMemGet(sizeof(goods_t));
    goods_t *goods = mem->addr;
    goods->mem = mem;
    goods->dinit = deinit;
    goods->setName = setName;

    goods->setName(goods, "goods_default");
    goods->price = 0;
    goods->buy_number = 0;

    goods->feild = goods;
    return goods;
}