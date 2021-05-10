#ifndef __VM_GUI__H
#define __VM_GUI__H
#include "dataMemory.h"

typedef struct VM_gui VMgui_t;

struct VM_gui
{
	DMEM *mem;
	int option_pointer;
	int option_pointer_max;
	int option_pointer_line;

	char option_str[5][32];

	VMgui_t *last_gui;
	VMgui_t *next_gui[5];

	void (*showString)(VMgui_t *gui, int x, int y, char *string);
	void (*refrash_optionPointer)(VMgui_t *gui);
	void (*refrash_optionStr)(VMgui_t *gui);
	void (*_option_callBack[5])(VMgui_t *gui);

	void (*change_gui)(VMgui_t *gui, VMgui_t *gui_new);

	void (*back_callBack)(VMgui_t *gui);

	char title[32];
	void (*refresh_title)(VMgui_t *gui);
	void (*_refresh_periodic)(VMgui_t *gui);

	void (*deinit)(VMgui_t *gui);
	void (*refresh)(VMgui_t *gui);
	void (*up)(VMgui_t *gui);
	void (*down)(VMgui_t *gui);
	void (*enter)(VMgui_t *gui);
	void (*back)(VMgui_t *gui);

	void (*_PORT_showLine)(VMgui_t *gui, int line, int cols, char *str);
	void (*_PORT_showString)(int x, int y, char *string);
	void (*_PORT_clear)(void);

	void (*clear)(VMgui_t *gui);
	// used to transfor diferent types of data to gui.
	void *generalData;

	// this parameter limited the context of Callback
	void *context;

	void (*setTitle)(VMgui_t *gui, char *title);
};

VMgui_t *VM_gui_init(void);

#endif
