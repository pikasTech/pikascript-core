#ifndef __panel_default_linux__H
#define __panel_default_linux__H
#include "VM_gui.h"
#include "dataMemory.h"

struct Data_observer
{
    int val;
    char *info;
};

typedef struct gui_default_linux panel_t;
struct gui_default_linux
{
    DMEM *mem;
    void (*dinit)(panel_t *gui_default_linux);
    VMgui_t *gui_home;
    VMgui_t *gui_observer;
    VMgui_t *gui_temperature;
    VMgui_t *gui_tumidity;
    VMgui_t *gui_smoke;
    VMgui_t *gui_carbon_monoxide;
    VMgui_t *gui_reconnect;

    VMgui_t *gui_main;

    void (*update)(panel_t *panel);
    struct Data_observer data_temperate;
    struct Data_observer data_tumidity;
    struct Data_observer data_smoke;
    struct Data_observer data_carbon_monoxide;
};
void panel_default_init(void);
#endif