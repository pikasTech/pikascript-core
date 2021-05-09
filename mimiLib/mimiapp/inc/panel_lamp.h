#ifndef _panel_lamp__H
#define _panel_lamp__H
#include "VM_gui.h"
#include "VM_memory.h"
typedef struct Class_panel_lamp panel_lamp_t;
struct Class_panel_lamp
{
    DMEM *mem;
    void (*dinit)(panel_lamp_t *panel);
    gui_t *gui_main;

    gui_t *gui_manual;
    gui_t *gui_auto;
    gui_t *gui_timing;
    gui_t *gui_modeSet;
    gui_t *gui_timerSet;

    void (*update)(panel_lamp_t *panel_lamp);
    void (*call_back_key1)(panel_lamp_t *panel_lamp);
    void (*call_back_key2)(panel_lamp_t *panel_lamp);
    void (*call_back_key3)(panel_lamp_t *panel_lamp);
    void (*refresh_periodic_observer)(gui_t *gui);
    void (*openLight_callBack)(gui_t *gui);
    void (*closeLight_callBack)(gui_t *gui);
    void (*enableInductor_callBack)(gui_t *gui);
    void (*disableInudctor_callBack)(gui_t *gui);

    void *context;
};
panel_lamp_t *Class_panel_lamp_init(panel_lamp_t *panel);
panel_lamp_t *NEW_Class_panel_lamp(void);
#endif