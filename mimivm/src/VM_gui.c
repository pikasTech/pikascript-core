#include "VM_gui.h"
#include "VM_memory.h"
#include "mimiStr.h"

void up(gui_t *gui)
{
	if (gui->option_pointer > 0)
	{
		gui->option_pointer--;
		gui->option_pointer_refrash(gui);

		return;
	}

	gui->option_pointer = gui->option_pointer_max;
	gui->option_pointer_refrash(gui);

	return;
}
void down(gui_t *gui)
{
	if (gui->option_pointer < gui->option_pointer_max)
	{
		gui->option_pointer++;
		gui->option_pointer_refrash(gui);
		return;
	}

	gui->option_pointer = 0;
	gui->option_pointer_refrash(gui);

	return;
}

void option_pointer_refrash(gui_t *gui)
{
	int pointer_line_new = gui->option_pointer * 1 + 1;
	gui->showLine(gui, gui->option_pointer_line, 1, "  ");
	gui->showLine(gui, pointer_line_new, 1, "->");
	gui->option_pointer_line = pointer_line_new;
}

void enter(gui_t *gui)
{
	gui->option_callBack[gui->option_pointer](gui);
}
void back(gui_t *gui)
{
	gui->back_callBack(gui);
}

static void deinit(gui_t *gui)
{
	DynMemPut(gui->mem);
}

static void refresh(gui_t *gui)
{
	gui->clear(gui);
	gui->showLine(gui, 6, 0, "----------------------");
	gui->showLine(gui, 7, 0, "|Back");
	gui->showLine(gui, 7, 0 + 5, "| Up");
	gui->showLine(gui, 7, 0 + 5 + 5, "|Down");
	gui->showLine(gui, 7, 0 + 5 + 5 + 5, "|Enter|");

	gui->option_pointer_refrash(gui);
	gui->option_str_refrash(gui);
	gui->title_refresh(gui);
}

static void option_str_refrash(gui_t *gui)
{
	int option_str_line;
	for (int i = 0; i < 5; i++)
	{
		option_str_line = i + 1;
		gui->showLine(gui, option_str_line, 5, gui->option_str[i]);
	}
}

static void title_refresh(gui_t *gui)
{
	gui->showLine(gui, 0, 1, gui->title);
}

static void callBack_default(gui_t *gui)
{
	//do nothing
}
static void PORT_VM_gui_showString(int x, int y, char *string)
{
	// rewrite here in port file.
}

static void showString(gui_t *gui, int x, int y, char *string)
{
	gui->PORT_VM_gui_showString(x, y, string);
}

static void clear(gui_t *gui)
{
	gui->PORT_VM_gui_clear();
}

static void PORT_VM_gui_clear(void)
{
}

static void change_gui(gui_t *gui, gui_t *gui_new)
{
	// log the last_gui to back
	gui_new->last_gui = gui;
	gui = gui_new;
	// refresh
	gui->refresh(gui);
}

static void back_callBack(gui_t *gui)
{
	//return to last gui
	gui = gui->last_gui;
	gui->refresh(gui);
}

static void refresh_periodic_deault(gui_t *gui)
{
	// rewrite here if need periodic refresh.
}

static void showLine(gui_t *gui, int line, int cols, char *str)
{
	// the map of line to x,y is for linux, rewrite it in other platform, shuch as lcd.
	gui->showString(gui, cols, line, str);
}

static void setTitle(gui_t *gui, char *title)
{
	for (int i = 0; i < 32; i++)
	{
		gui->title[i] = 0;
	}
	strPrint(gui->title, title);
}

gui_t *VM_gui_init(void)
{
	DMEM *mem = DynMemGet(sizeof(gui_t));
	gui_t *gui = (gui_t *)mem->addr;
	gui->mem = mem;
	gui->deinit = deinit;
	gui->refresh = refresh;
	gui->up = up;
	gui->down = down;
	gui->enter = enter;
	gui->back = back;
	gui->last_gui = gui;
	gui->change_gui = change_gui;
	gui->option_pointer = 0;
	gui->option_pointer_line = 0;
	gui->option_pointer_max = 4;
	gui->option_pointer_refrash = option_pointer_refrash;
	gui->option_str_refrash = option_str_refrash;
	gui->showString = showString;

	gui->showLine = showLine;

	gui->PORT_VM_gui_showString = PORT_VM_gui_showString;

	gui->refresh_periodic = refresh_periodic_deault;
	for (int i = 0; i < 5; i++)
	{
		gui->option_str[i][0] = 0;
		gui->option_callBack[i] = callBack_default;
		gui->next_gui[i] = gui;
	}
	gui->title[0] = 0;
	gui->title_refresh = title_refresh;
	gui->back_callBack = back_callBack;

	gui->PORT_VM_gui_clear = PORT_VM_gui_clear;
	gui->clear = clear;

	gui->generalData = NULL;

	// the default context id object itself.
	// the context can be expand when the object is
	// containd in other object.
	gui->context = (void *)gui;
	gui->setTitle = setTitle;
	return gui;
}