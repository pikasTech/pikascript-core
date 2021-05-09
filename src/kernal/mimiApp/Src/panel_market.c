#include "panel_market.h"
#include "VM_gui.h"
#include "VM_key.h"
#include "VM_memory.h"
#include "mimiPort_market.h"
#include "mimiStr.h"

static void option_pointer_refrash_void(gui_t *gui);

static void deinit(panel_market_t *panel_market)
{
    DynMemPut(panel_market->mem);
    panel_market->gui_home->deinit(panel_market->gui_home);
    panel_market->gui_main->deinit(panel_market->gui_main);
}

void guiChange_callBack(void *context)
{
    // get the context
    panel_market_t *panel_market = (panel_market_t *)context;

    // log gui Now
    gui_t *guiNow = panel_market->gui_main;
    gui_t *guiNext = guiNow->next_gui[guiNow->option_pointer];
    // change gui_main
    panel_market->gui_main = guiNext;
    panel_market->gui_main->refresh(panel_market->gui_main);
    panel_market->gui_main->last_gui = guiNow;
}

static void back_callBack(void *context)
{
    panel_market_t *panel_market = (panel_market_t *)context;
    gui_t *guiNow = panel_market->gui_main;
    gui_t *guiLast = guiNow->last_gui;

    panel_market->gui_main = guiLast;

    panel_market->gui_main->refresh(panel_market->gui_main);
}

static void refresh_Next(gui_t *gui)
{
    gui->clear(gui);
    gui->showLine(gui, 6, 0, "----------------------");
    gui->showLine(gui, 7, 0, "|Back");
    gui->showLine(gui, 7, 0 + 5, "|   Next   ");
    gui->showLine(gui, 7, 0 + 5 + 5 + 5, "|Enter|");

    gui->option_pointer_refrash(gui);
    gui->option_str_refrash(gui);
    gui->title_refresh(gui);
}

static void gui_init_home(gui_t *gui, void *context)
{
    gui->option_pointer_max = 3;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->refresh = refresh_Next;
    gui->refresh_periodic = gui->refresh;
    panel_market_t *panel_market = (panel_market_t *)gui->context;
    strPrint(gui->title, "Home");

    strPrint(gui->option_str[0], "Shopping");
    gui->next_gui[0] = panel_market->gui_shopping;
    gui->option_callBack[0] = guiChange_callBack;

    strPrint(gui->option_str[1], "Pay");
    gui->next_gui[1] = panel_market->gui_Pay;
    gui->option_callBack[1] = guiChange_callBack;

    strPrint(gui->option_str[2], "Set Price");
    gui->next_gui[2] = panel_market->gui_Set;
    gui->option_callBack[2] = guiChange_callBack;

    strPrint(gui->option_str[3], "Reconnect");
    gui->next_gui[3] = panel_market->gui_Reconnect;
    gui->option_callBack[3] = guiChange_callBack;
}

static void gui_init_shopping(gui_t *gui, void *context)
{
    gui->option_pointer_max = 4;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->refresh = refresh_Next;

    panel_market_t *panel = (panel_market_t *)context;

    strPrint(gui->title, "Shopping");

    strPrint(gui->option_str[0], "goods 1");
    gui->next_gui[0] = panel->gui_goods_shopping;
    gui->option_callBack[0] = guiChange_callBack;

    strPrint(gui->option_str[1], "goods 2");
    gui->next_gui[1] = panel->gui_goods_shopping;
    gui->option_callBack[1] = guiChange_callBack;

    strPrint(gui->option_str[2], "goods 3");
    gui->next_gui[2] = panel->gui_goods_shopping;
    gui->option_callBack[2] = guiChange_callBack;

    strPrint(gui->option_str[3], "goods 4");
    gui->next_gui[3] = panel->gui_goods_shopping;
    gui->option_callBack[3] = guiChange_callBack;

    strPrint(gui->option_str[4], "goods 5");
    gui->next_gui[4] = panel->gui_goods_shopping;
    gui->option_callBack[4] = guiChange_callBack;
}

static void gui_init_set(gui_t *gui, void *context)
{
    gui->option_pointer_max = 4;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->refresh = refresh_Next;
    strPrint(gui->title, "Set Price");

    panel_market_t *panel = (panel_market_t *)context;

    strPrint(gui->option_str[0], "goods 1");
    gui->next_gui[0] = panel->gui_goods_set;
    gui->option_callBack[0] = guiChange_callBack;

    strPrint(gui->option_str[1], "goods 2");
    gui->next_gui[1] = panel->gui_goods_set;
    gui->option_callBack[1] = guiChange_callBack;

    strPrint(gui->option_str[2], "goods 3");
    gui->next_gui[2] = panel->gui_goods_set;
    gui->option_callBack[2] = guiChange_callBack;

    strPrint(gui->option_str[3], "goods 4");
    gui->next_gui[3] = panel->gui_goods_set;
    gui->option_callBack[3] = guiChange_callBack;

    strPrint(gui->option_str[4], "goods 5");
    gui->next_gui[4] = panel->gui_goods_set;
    gui->option_callBack[4] = guiChange_callBack;
}
static void gui_init_pay(gui_t *gui, void *context)
{
    gui->option_pointer_max = 4;
    gui->context = context;
    gui->back_callBack = back_callBack;
    // don't show the pointer
    gui->option_pointer_refrash = option_pointer_refrash_void;
    gui->refresh = refresh_Next;
    strPrint(gui->title, "Pay");
}
static void gui_init_reconnect(gui_t *gui, void *context)
{
    gui->option_pointer_max = 4;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->option_pointer_refrash = option_pointer_refrash_void;
    gui->refresh = refresh_Next;
    strPrint(gui->title, "Reconnect");
}

static void option_pointer_refrash_void(gui_t *gui)
{
    //do not refrash the option_pointer
}

static void gui_init_goods_set(gui_t *gui, void *context)
{
    gui->option_pointer_max = 0;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->option_pointer_refrash = option_pointer_refrash_void;
    gui->refresh = refresh_Next;
    strPrint(gui->title, "Title_default");
}

static void gui_init_goods_shopping(gui_t *gui, void *context)
{
    gui->option_pointer_max = 0;
    gui->context = context;
    gui->back_callBack = back_callBack;
    gui->option_pointer_refrash = option_pointer_refrash_void;
    gui->refresh = refresh_Next;
    strPrint(gui->title, "Title_default");
}
static void update(panel_market_t *panel_market)
{
    panel_market->gui_main->refresh_periodic(panel_market->gui_main);
}

static void call_back_key1(void *context)
{
    panel_market_t *panel_market = (panel_market_t *)context;
    gui_t *gui = panel_market->gui_main;
    gui->back(gui);
}
static void call_back_key2(void *context)
{
    panel_market_t *panel_market = (panel_market_t *)context;
    gui_t *gui = panel_market->gui_main;
    gui->down(gui);
}
static void call_back_key3(void *context)
{
    panel_market_t *panel_market = (panel_market_t *)context;
    gui_t *gui = panel_market->gui_main;
    gui->enter(gui);
}

panel_market_t *Class_panel_market_init(void)
{
    DMEM *mem = DynMemGet(sizeof(panel_market_t));
    panel_market_t *panel_market = mem->addr;
    panel_market->mem = mem;
    panel_market->dinit = deinit;

    panel_market->gui_goods_set = VM_gui_init_PORT();
    panel_market->gui_goods_shopping = VM_gui_init_PORT();
    panel_market->gui_shopping = VM_gui_init_PORT();
    panel_market->gui_Set = VM_gui_init_PORT();
    panel_market->gui_Pay = VM_gui_init_PORT();
    panel_market->gui_Reconnect = VM_gui_init_PORT();
    panel_market->gui_home = VM_gui_init_PORT();

    gui_init_goods_set(panel_market->gui_goods_set, panel_market);
    gui_init_goods_shopping(panel_market->gui_goods_shopping, panel_market);
    gui_init_shopping(panel_market->gui_shopping, panel_market);
    gui_init_set(panel_market->gui_Set, panel_market);
    gui_init_pay(panel_market->gui_Pay, panel_market);
    gui_init_reconnect(panel_market->gui_Reconnect, panel_market);
    gui_init_home(panel_market->gui_home, panel_market);

    panel_market->call_back_key1 = call_back_key1;
    panel_market->call_back_key2 = call_back_key2;
    panel_market->call_back_key3 = call_back_key3;

    // set the first gui to be showen
    // gui_main is just a pointer
    panel_market->gui_main = panel_market->gui_home;

    panel_market->update = update;

    // set deafult context to panel_market;
    // if need to react with in bigger object, the context
    // can be set as the bigger one.
    panel_market->context = panel_market;
    return panel_market;
}