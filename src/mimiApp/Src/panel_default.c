#include "panel_default.h"
#include "VM_gui.h"
#include "VM_memory.h"

#include "VM_delay.h"
#include "mimiPort_market.h"
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
#ifndef LINUX
#include "main.h"
#endif
panel_t *Pub_panel;

static void change_gui(gui_t *gui, gui_t *gui_new)
{
	Pub_panel->gui_main = gui_new;
	Pub_panel->gui_main->refresh(Pub_panel->gui_main);
	// log the last_gui to back
	Pub_panel->gui_main->last_gui = gui;
}

static void back_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	Pub_panel->gui_main = gui->last_gui;
	Pub_panel->gui_main->refresh(Pub_panel->gui_main);
}

static void reboot_callBack(void *context)
{
	// reboot
#ifndef LINUX
	HAL_NVIC_SystemReset();
#endif
}

static void observer_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_observer);
}

static void temperature_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_temperature);
}

static void tumidity_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_tumidity);
}

static void reconnect_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_reconnect);
	gui->showString(gui, 1, 1, "Connecting to ");
	gui->showString(gui, 1, 2, "iot server......");
	DynMemPut((DMEM *)(PubSh->cmd(PubSh, "iot connect")));
	gui->showString(gui, 10, 3, "Reconnect finished!");
	gui->showString(gui, 10, 4, "Please press [Back].");
}

static void smoke_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_smoke);
}

static void gui_carbon_monoxide_callBack(void *context)
{
	gui_t *gui = (gui_t *)context;
	gui->change_gui(gui, Pub_panel->gui_carbon_monoxide);
}

static void option_pointer_refrash_void(gui_t *gui)
{
	//do not refrash the option_pointer
}

static void refresh_periodic_Observer_value(gui_t *gui)
{
	//get data from general data
	struct Data_observer *data = (struct Data_observer *)gui->generalData;

	char str[32] = {0};
	// clear the string
	gui->showString(gui, 4, 5, "            ");
	sprintf(str, data->info, data->val);
	gui->showString(gui, 4, 5, str);
}

gui_t *VM_gui_init_PORT_panel(void)
{
	gui_t *gui = VM_gui_init_PORT();
	gui->back_callBack = back_callBack;
	gui->change_gui = change_gui;
	return gui;
}

static gui_t *gui_Observer_value_init(char *title, void *general_data)
{
	gui_t *gui;
	gui = VM_gui_init_PORT_panel();
	gui->option_pointer_refrash = option_pointer_refrash_void;
	gui->option_pointer_max = 0;
	gui->refresh_periodic = refresh_periodic_Observer_value;
	gui->generalData = general_data;
	strPrint(gui->title, title);
	return gui;
}

static void gui_Observer_init(void)
{
	Pub_panel->gui_observer = VM_gui_init_PORT_panel();
	gui_t *gui = Pub_panel->gui_observer;
	gui->option_pointer_max = 3;
	strPrint(gui->option_str[0], "Temperature");
	gui->option_callBack[0] = temperature_callBack;

	strPrint(gui->option_str[1], "Tumidity");
	gui->option_callBack[1] = tumidity_callBack;

	strPrint(gui->option_str[2], "Smoke");
	gui->option_callBack[2] = smoke_callBack;

	strPrint(gui->option_str[3], "Carbon monoxide");
	gui->option_callBack[3] = gui_carbon_monoxide_callBack;

	strPrint(gui->title, "Observer");
}

static void gui_home_init(panel_t *panel)
{
	panel->gui_home = VM_gui_init_PORT_panel();
	gui_t *gui = panel->gui_home;
	gui->option_pointer_max = 2;
	strPrint(gui->option_str[0], "Observer");
	gui->option_callBack[0] = observer_callBack;
	strPrint(gui->option_str[1], "Reconnect");
	gui->option_callBack[1] = reconnect_callBack;
	strPrint(gui->option_str[2], "Reboot");
	gui->option_callBack[2] = reboot_callBack;
	strPrint(gui->title, "Home");
}

static void refresh_periodic_deault(gui_t *gui)
{
	// rewrite here if need periodic refresh.
}

static void panel_init_PORT(panel_t *panel)
{
	gui_home_init(Pub_panel);
	gui_Observer_init();
	panel->gui_temperature = gui_Observer_value_init("Temperature", (void *)&panel->data_temperate);
	panel->gui_tumidity = gui_Observer_value_init("Tumidity", (void *)&panel->data_tumidity);

	panel->gui_reconnect = gui_Observer_value_init("Reconnect", NULL);
	panel->gui_reconnect->refresh_periodic = refresh_periodic_deault;

	panel->gui_smoke = gui_Observer_value_init("Smoke", (void *)&panel->data_smoke);
	panel->gui_carbon_monoxide = gui_Observer_value_init("Carbon monoxide", (void *)&panel->data_carbon_monoxide);
	// chose the root gui
	panel->gui_main = panel->gui_home;
}

static void deinit(panel_t *panel)
{
	DynMemPut(panel->mem);
}

static void update(panel_t *panel)
{
	panel->gui_main->refresh_periodic(panel->gui_main);
}

void panel_default_init(void)
{
	DMEM *mem = DynMemGet(sizeof(panel_t));
	panel_t *panel = mem->addr;

	panel->update = update;

	panel->mem = mem;
	panel->dinit = deinit;
	panel->data_carbon_monoxide.info = "Val: %d\r\n";
	panel->data_smoke.info = "Val: %d\r\n";
	panel->data_temperate.info = "Val: %d\r\n";
	panel->data_tumidity.info = "Val: %d\r\n";
	panel->data_carbon_monoxide.val = 0;
	panel->data_smoke.val = 0;
	panel->data_temperate.val = 0;
	panel->data_tumidity.val = 0;

	Pub_panel = panel;

	panel_init_PORT(Pub_panel);
}