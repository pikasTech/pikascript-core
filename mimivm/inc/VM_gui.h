#ifndef __VM_GUI__H
#define __VM_GUI__H
#include "VM_memory.h"

typedef struct VM_gui gui_t;

struct VM_gui
{
	DMEM *mem;
	int option_pointer;
	int option_pointer_max;
	int option_pointer_line;

	char option_str[5][32];

	gui_t *last_gui;
	gui_t *next_gui[5];

	void (*showString)(gui_t *gui, int x, int y, char *string);
	void (*option_pointer_refrash)(gui_t *gui);
	void (*option_str_refrash)(gui_t *gui);
	void (*option_callBack[5])(gui_t *gui);

	void (*change_gui)(gui_t *gui, gui_t *gui_new);

	void (*back_callBack)(gui_t *gui);

	char title[32];
	void (*title_refresh)(gui_t *gui);
	void (*refresh_periodic)(gui_t *gui);

	void (*deinit)(gui_t *gui);
	void (*refresh)(gui_t *gui);
	void (*up)(gui_t *gui);
	void (*down)(gui_t *gui);
	void (*enter)(gui_t *gui);
	void (*back)(gui_t *gui);

	void (*showLine)(gui_t *gui, int line, int cols, char *str);
	void (*PORT_VM_gui_showString)(int x, int y, char *string);
	void (*PORT_VM_gui_clear)(void);

	void (*clear)(gui_t *gui);
	// used to transfor diferent types of data to gui.
	void *generalData;

	// this parameter limited the context of Callback 
	void *context;

void (*setTitle)(gui_t *gui, char *title);

};

gui_t *VM_gui_init(void);

#endif
