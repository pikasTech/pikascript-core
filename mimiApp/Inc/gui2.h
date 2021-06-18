#ifndef _gui2__H
#define _gui2__H
#include "dataArgs.h"
#include "dataMemory.h"
typedef struct Class_gui gui2_t;
struct Class_gui
{
    /* attribute */
    DMEM *mem;
    void *context;
    void *generalData;
    int option_pointer;
    int option_pointer_max;
    int option_pointer_line;
    char option_str[5][32];
    char title[32];
    int dataIndex;
    gui2_t *last_gui;
    gui2_t *next_gui[5];

    /* operation */
    void (*dinit)(gui2_t *self);
    void (*init)(gui2_t *self, args_t *args);
    void (*refresh)(gui2_t *gui);
    void (*refrash_optionPointer)(gui2_t *gui);
    void (*refrash_optionStr)(gui2_t *gui);
    void (*refresh_title)(gui2_t *gui);
    void (*change_gui)(gui2_t *gui, gui2_t *gui_new);
    void (*back_callBack)(gui2_t *gui);
    void (*up)(gui2_t *gui);
    void (*down)(gui2_t *gui);
    void (*enter)(gui2_t *gui);
    void (*back)(gui2_t *gui);
    void (*showLine)(gui2_t *gui, int line, int cols, char *str);
    void (*showString)(gui2_t *gui, int x, int y, char *string);
    void (*clear)(gui2_t *gui);
    void (*setTitle)(gui2_t *gui, char *title);
    void (*argHandle)(gui2_t *self,
                      args_t *args,
                      char *argName,
                      void (*handle)(gui2_t *self,
                                     args_t *args,
                                     char *argName));
    void (*guiChange_callBack)(gui2_t *self);

    /* virtual operation */
    void (*_refresh_periodic)(gui2_t *gui);
    void (*_option_callBack[5])(gui2_t *gui);

    /* object */
};

gui2_t *New_gui2(args_t *args);
#endif