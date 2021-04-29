#ifndef _panel_market__H
#define _panel_market__H
#include "VM_memory.h"
#include "VM_gui.h"
typedef struct Class_panel_market panel_market_t;
struct Class_panel_market
{
    DMEM *mem;
    void (*dinit)(panel_market_t *panel_market);
    gui_t *gui_home;
    gui_t *gui_main;

    gui_t *gui_shopping;
    gui_t *gui_Reconnect;
    gui_t *gui_Pay;
    gui_t *gui_Set;

    gui_t *gui_goods_set;
    gui_t *gui_goods_shopping;

    void (*call_back_key1)(void *context);
    void (*call_back_key2)(void *context);
    void (*call_back_key3)(void *context);

    void (*update)(panel_market_t * panel_market);
    void *context;
};
panel_market_t *Class_panel_market_init(void);

void guiChange_callBack(void *context);
#endif