#ifndef __panel_default_linux__H
#define __panel_default_linux__H
#include "VM_gui.h"
#include "VM_memory.h"

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
    gui_t *gui_home;
    gui_t *gui_observer;
    gui_t *gui_temperature;
    gui_t *gui_tumidity;
    gui_t *gui_smoke;
    gui_t *gui_carbon_monoxide;
    gui_t *gui_reconnect;

    gui_t *gui_main;

    void (*update)(panel_t *panel);
    struct Data_observer data_temperate;
    struct Data_observer data_tumidity;
    struct Data_observer data_smoke;
    struct Data_observer data_carbon_monoxide;
};
void panel_default_init(void);
#endif