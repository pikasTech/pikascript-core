#ifndef _panel_lamp__H
#define _panel_lamp__H
#include "VM_gui.h"
#include "dataMemory.h"
typedef struct Class_panel_lamp panel_lamp_t;
struct Class_panel_lamp
{
    DMEM *mem;
    void (*dinit)(panel_lamp_t *panel);
    VMgui_t *gui_main;

    VMgui_t *gui_manual;
    VMgui_t *gui_auto;
    VMgui_t *gui_timing;
    VMgui_t *gui_modeSet;
    VMgui_t *gui_timerSet;

    void (*update)(panel_lamp_t *panel_lamp);
    void (*call_back_key1)(panel_lamp_t *panel_lamp);
    void (*call_back_key2)(panel_lamp_t *panel_lamp);
    void (*call_back_key3)(panel_lamp_t *panel_lamp);
    void (*refresh_periodic_observer)(VMgui_t *gui);
    void (*openLight_callBack)(VMgui_t *gui);
    void (*closeLight_callBack)(VMgui_t *gui);
    void (*enableInductor_callBack)(VMgui_t *gui);
    void (*disableInudctor_callBack)(VMgui_t *gui);

    void *context;
};
panel_lamp_t *Class_panel_lamp_init(panel_lamp_t *panel);
panel_lamp_t *NEW_Class_panel_lamp(void);
#endif