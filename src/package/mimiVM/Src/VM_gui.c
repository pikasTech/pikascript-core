#include "VM_gui.h"
#include "dataMemory.h"
#include "dataString.h"

void up(VMgui_t *gui)
{
	if (gui->option_pointer > 0)
	{
		gui->option_pointer--;
		gui->refrash_optionPointer(gui);

		return;
	}

	gui->option_pointer = gui->option_pointer_max;
	gui->refrash_optionPointer(gui);

	return;
}
void down(VMgui_t *gui)
{
	if (gui->option_pointer < gui->option_pointer_max)
	{
		gui->option_pointer++;
		gui->refrash_optionPointer(gui);
		return;
	}

	gui->option_pointer = 0;
	gui->refrash_optionPointer(gui);

	return;
}

void refrash_optionPointer(VMgui_t *gui)
{
	int pointer_line_new = gui->option_pointer * 1 + 1;
	gui->_PORT_showLine(gui, gui->option_pointer_line, 1, "  ");
	gui->_PORT_showLine(gui, pointer_line_new, 1, "->");
	gui->option_pointer_line = pointer_line_new;
}

void enter(VMgui_t *gui)
{
	gui->_option_callBack[gui->option_pointer](gui);
}
void back(VMgui_t *gui)
{
	gui->back_callBack(gui);
}

static void deinit(VMgui_t *gui)
{
	DynMemPut(gui->mem);
}

static void refresh(VMgui_t *gui)
{
	gui->clear(gui);
	gui->_PORT_showLine(gui, 6, 0, "----------------------");
	gui->_PORT_showLine(gui, 7, 0, "|Back");
	gui->_PORT_showLine(gui, 7, 0 + 5, "| Up");
	gui->_PORT_showLine(gui, 7, 0 + 5 + 5, "|Down");
	gui->_PORT_showLine(gui, 7, 0 + 5 + 5 + 5, "|Enter|");

	gui->refrash_optionPointer(gui);
	gui->refrash_optionStr(gui);
	gui->refresh_title(gui);
}

static void refrash_optionStr(VMgui_t *gui)
{
	int option_str_line;
	for (int i = 0; i < 5; i++)
	{
		option_str_line = i + 1;
		gui->_PORT_showLine(gui, option_str_line, 5, gui->option_str[i]);
	}
}

static void refresh_title(VMgui_t *gui)
{
	gui->_PORT_showLine(gui, 0, 1, gui->title);
}

static void _callBack_default(VMgui_t *gui)
{
	//do nothing
}
static void _PORT_showString(int x, int y, char *string)
{
	// rewrite here in port file.
}

static void showString(VMgui_t *gui, int x, int y, char *string)
{
	gui->_PORT_showString(x, y, string);
}

static void clear(VMgui_t *gui)
{
	gui->_PORT_clear();
}

static void _PORT_clear(void)
{
}

static void change_gui(VMgui_t *gui, VMgui_t *gui_new)
{
	// log the last_gui to back
	gui_new->last_gui = gui;
	gui = gui_new;
	// refresh
	gui->refresh(gui);
}

static void back_callBack(VMgui_t *gui)
{
	//return to last gui
	gui = gui->last_gui;
	gui->refresh(gui);
}

static void _refresh_periodic_deault(VMgui_t *gui)
{
	// rewrite here if need periodic refresh.
}

static void _PORT_showLine(VMgui_t *gui, int line, int cols, char *str)
{
	// the map of line to x,y is for linux, rewrite it in other platform, shuch as lcd.
	gui->showString(gui, cols, line, str);
}

static void setTitle(VMgui_t *gui, char *title)
{
	for (int i = 0; i < 32; i++)
	{
		gui->title[i] = 0;
	}
	strPrint(gui->title, title);
}

VMgui_t *VM_gui_init(void)
{
	DMEM *mem = DynMemGet(sizeof(VMgui_t));
	VMgui_t *gui = (VMgui_t *)mem->addr;
	gui->mem = mem;
	gui->deinit = deinit;

	/* attrivute */
	gui->title[0] = 0;
	gui->last_gui = gui;
	gui->generalData = NULL;
	gui->option_pointer = 0;
	gui->context = (void *)gui;
	gui->option_pointer_max = 4;
	gui->option_pointer_line = 0;
	for (int i = 0; i < 5; i++)
	{
		gui->option_str[i][0] = 0;
		gui->next_gui[i] = gui;
	}

	/* operation */

	// api
	gui->up = up;
	gui->down = down;
	gui->back = back;
	gui->enter = enter;
	gui->clear = clear;
	gui->refresh = refresh;
	gui->_PORT_showLine = _PORT_showLine;
	gui->setTitle = setTitle;
	gui->change_gui = change_gui;
	gui->showString = showString;
	gui->back_callBack = back_callBack;
	gui->refresh_title = refresh_title;
	gui->refrash_optionStr = refrash_optionStr;
	gui->refrash_optionPointer = refrash_optionPointer;

	/* virtual operation */
	gui->_PORT_clear = _PORT_clear;
	gui->_refresh_periodic = _refresh_periodic_deault;
	gui->_PORT_showString = _PORT_showString;
	for (int i = 0; i < 5; i++)
	{
		gui->_option_callBack[i] = _callBack_default;
	}

	// the default context id object itself.
	// the context can be expand when the object is
	// containd in other object.
	return gui;
}