#include "panel_lamp.h"
#include "VM_memory.h"
#include "mimiPort_market.h"
#include "mimiStr.h"

static void guiChange_callBack(gui_t *gui);

static void deinit(panel_lamp_t *panel)
{
    DynMemPut(panel->mem);
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

static void refresh_periodic_void(gui_t *gui)
{
}

static void back_callBack(gui_t *gui)
{
    panel_lamp_t *panel = (panel_lamp_t *)gui->context;
    gui_t *guiNow = panel->gui_main;
    gui_t *guiLast = guiNow->last_gui;

    panel->gui_main = guiLast;

    panel->gui_main->refresh(panel->gui_main);
}

gui_t *VM_gui_init_lamp_base(void *context, char *title)
{
    gui_t *gui = VM_gui_init_PORT();

    gui->back_callBack = back_callBack;
    gui->context = context;
    gui->refresh = refresh_Next;
    gui->refresh_periodic = refresh_periodic_void;
    gui->setTitle(gui, title);

    return gui;
}

gui_t *VM_gui_init_lamp_observer(void *context, char *title)
{
    gui_t *gui = VM_gui_init_lamp_base(context, title);
    panel_lamp_t *panel = gui->context;
    gui->refresh_periodic = panel->refresh_periodic_observer;

    return gui;
}

gui_t *VM_gui_init_lamp_timerSet(void *context, char *title)
{
    gui_t *gui = VM_gui_init_lamp_observer(context, title);

    gui->option_pointer_max = 0;
    gui->option_pointer_refrash = refresh_periodic_void;

    return gui;
}

gui_t *VM_gui_init_lamp_manual(void *context, char *title)
{
    gui_t *gui = VM_gui_init_lamp_observer(context, title);
    panel_lamp_t *panel = gui->context;

    gui->option_pointer_max = 1;

    strPrint(gui->option_str[0], "open light");
    gui->option_callBack[0] = panel->openLight_callBack;

    strPrint(gui->option_str[1], "close ligh");
    gui->option_callBack[1] = panel->closeLight_callBack;

    return gui;
}

gui_t *VM_gui_init_lamp_auto(void *context,char *title)
{
    gui_t *gui = VM_gui_init_lamp_observer(context, title);
    panel_lamp_t *panel = gui->context;

    gui->option_pointer_max = 1;

    strPrint(gui->option_str[0], "enable auto");
    gui->option_callBack[0] = panel->enableInductor_callBack;

    strPrint(gui->option_str[1], "disable auto");
    gui->option_callBack[1] = panel->disableInudctor_callBack;

    return gui;
}

gui_t *VM_gui_init_lamp_timing(void *context,char *title)
{
    gui_t *gui = VM_gui_init_lamp_observer(context, title);
    panel_lamp_t *panel = gui->context;

    gui->option_pointer_max = 1;

    strPrint(gui->option_str[0], "set timer");
    gui->next_gui[0] = panel->gui_timerSet;
    gui->option_callBack[0] = guiChange_callBack;

    strPrint(gui->option_str[1], "start timer");
    gui->option_callBack[1] = panel->disableInudctor_callBack;

    return gui;
}

static void guiChange_callBack(gui_t *gui)
{
    // get the context
    panel_lamp_t *panel = (panel_lamp_t *)gui->context;

    // log gui Now
    gui_t *guiNow = panel->gui_main;
    gui_t *guiNext = guiNow->next_gui[guiNow->option_pointer];
    // change gui_main
    panel->gui_main = guiNext;
    panel->gui_main->refresh(panel->gui_main);
    panel->gui_main->last_gui = guiNow;
}

gui_t *VM_gui_init_lamp_modeSet(void *context, char *title)
{
    panel_lamp_t *panel = (panel_lamp_t *)context;
    gui_t *gui = VM_gui_init_lamp_base(context, title);
    gui->option_pointer_max = 2;

    strPrint(gui->option_str[0], "manual mode");
    gui->next_gui[0] = panel->gui_manual;
    gui->option_callBack[0] = guiChange_callBack;

    strPrint(gui->option_str[1], "auto mode");
    gui->next_gui[1] = panel->gui_auto;
    gui->option_callBack[1] = guiChange_callBack;

    strPrint(gui->option_str[2], "timing mode");
    gui->next_gui[2] = panel->gui_timing;
    gui->option_callBack[2] = guiChange_callBack;

    //gui->refresh_periodic = gui->refresh;
    return gui;
}

static void update(panel_lamp_t *panel_lamp)
{
    panel_lamp->gui_main->refresh_periodic(panel_lamp->gui_main);
}

void call_back_key1(panel_lamp_t *panel_lamp)
{
    panel_lamp->gui_main->back(panel_lamp->gui_main);
}
void call_back_key2(panel_lamp_t *panel_lamp)
{
    panel_lamp->gui_main->down(panel_lamp->gui_main);
}
void call_back_key3(panel_lamp_t *panel_lamp)
{
    panel_lamp->gui_main->enter(panel_lamp->gui_main);
}

static void default_callBack(gui_t *gui)
{
    //rewrite in user code
}



panel_lamp_t *NEW_Class_panel_lamp(void)
{
    DMEM *mem = DynMemGet(sizeof(panel_lamp_t));
    panel_lamp_t *panel = mem->addr;
    panel->mem = mem;
    panel->dinit = deinit;
    panel->refresh_periodic_observer = refresh_periodic_void;
    panel->closeLight_callBack = default_callBack;
    panel->openLight_callBack = default_callBack;
    panel->context = panel;
    return panel;
}

panel_lamp_t *Class_panel_lamp_init(panel_lamp_t *panel)
{
    panel->gui_timerSet = VM_gui_init_lamp_timerSet(panel, "set timer");
    panel->gui_manual = VM_gui_init_lamp_manual(panel, "manel mode");
    panel->gui_auto = VM_gui_init_lamp_auto(panel, "auto mode");
    panel->gui_timing = VM_gui_init_lamp_timing(panel, "timing mode");
    panel->gui_modeSet = VM_gui_init_lamp_modeSet(panel, "mode select");

    panel->gui_main = panel->gui_modeSet;

    panel->call_back_key1 = call_back_key1;
    panel->call_back_key2 = call_back_key2;
    panel->call_back_key3 = call_back_key3;

    panel->update = update;
    return panel;
}
