#include "dataArgs.h"
#include "dataMemory.h"
#include "server.h"
#include "gui_3key.h"
#include "gui_set.h"
#include "gui_watch.h"
#include "VM_key.h"
#include "mimiPort_market.h"

server_t *panel;

static void update(server_t *self, int systime)
{
    gui2_t *gui_main = self->subObjectList[0];
		vkey_t *key_0 = self->subObjectList[1];
		vkey_t *key_1 = self->subObjectList[2];
		vkey_t *key_2 = self->subObjectList[3];

    // return if lamp is not enable
    if (0 == self->isEnable)
    {
        return;
    }

    if (0 == systime % 500)
    {
        gui_main->_refresh_periodic(gui_main);
    }
		
		if (0 == systime % 50)
    {
        key_0->update_port(key_0, systime);
        key_1->update_port(key_1, systime);
        key_2->update_port(key_2, systime);
    }
    if (0 == systime % 300)
    {
        key_0->update_callBack(key_0, systime);
        key_1->update_callBack(key_1, systime);
        key_2->update_callBack(key_2, systime);
    }
}

static void enable(server_t *self)
{
    self->isEnable = 1;
    gui2_t *gui_main = self->subObjectList[0];
    gui_main->refresh(gui_main);
}


// callback need to be write in user code.
static void callback_key_0(vkey_t *key, void *arg)
{
    server_t *lamp = key->context;
    gui2_t *gui = lamp->subObjectList[0];
    gui->back(gui);
}

// callback need to be write in user code.
static void callback_key_1(vkey_t *key, void *arg)
{
    server_t *lamp = key->context;
    gui2_t *gui = lamp->subObjectList[0];
		gui->down(gui);
}

// callback need to be write in user code.
static void callback_key_2(vkey_t *key, void *arg)
{
    server_t *lamp = key->context;
    gui2_t *gui = lamp->subObjectList[0];
		gui->enter(gui);
}


static vkey_t *VM_key_init_lamp(int KEY_id, void *context)
{
    vkey_t *key = VM_key_init_PORT(KEY_id);
    switch (KEY_id)
    {
    case 0:
        key->callback = callback_key_0;
        break;
    case 1:
        key->callback = callback_key_1;
        break;
    case 2:
        key->callback = callback_key_2;
        break;
    }
    key->context = context;
    return key;
}


static void init_panelTemp(server_t *self, args_t *args)
{
    /* attrivute */

    /* operation */
    self->enable = enable;
		self->update = update;

    /* object */
    gui2_t *gui_main;
    gui2_t *gui_home;
    gui2_t *gui_observer;
    gui2_t *gui_setAlert;
    gui2_t *gui_watch0;
    gui2_t *gui_watch1;
    gui2_t *gui_watch2;
    gui2_t *gui_watch3;
    gui2_t *gui_set0;
    gui2_t *gui_set1;
	  vkey_t * key_0 = VM_key_init_lamp(0, self);
    vkey_t * key_1 = VM_key_init_lamp(1, self);
    vkey_t * key_2 = VM_key_init_lamp(2, self);
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "home");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 1);
        args->setStr(args, "optionStr0", "watch val");
        args->setStr(args, "optionStr1", "set allert");
        args->setStr(args, "optionCallBack0", "guiChange");
        args->setStr(args, "optionCallBack1", "guiChange");

        gui_home = New_gui2_3key(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "select a val");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 3);
        args->setStr(args, "optionStr0", "temp1");
        args->setStr(args, "optionStr1", "temp2");
        args->setStr(args, "optionStr2", "hum1");
        args->setStr(args, "optionStr3", "hum2");
        args->setStr(args, "optionCallBack0", "guiChange");
        args->setStr(args, "optionCallBack1", "guiChange");
        args->setStr(args, "optionCallBack2", "guiChange");
        args->setStr(args, "optionCallBack3", "guiChange");

        gui_observer = New_gui2_3key(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "select a val");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 1);
        args->setStr(args, "optionStr0", "temp1");
        args->setStr(args, "optionStr1", "temp2");
        args->setStr(args, "optionCallBack0", "guiChange");
        args->setStr(args, "optionCallBack1", "guiChange");

        gui_setAlert = New_gui2_3key(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "temp1");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 0);

        gui_watch0 = New_gui2_watch(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "temp2");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 1);

        gui_watch1 = New_gui2_watch(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "hum1");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 2);

        gui_watch2 = New_gui2_watch(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "hum2");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 3);

        gui_watch3 = New_gui2_watch(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "temp1 allert");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 4);

        gui_set0 = New_gui2_set(args);

        args->dinit(args);
    }
    {
        args_t *args = New_args(NULL);
        args->setStr(args, "title", "temp2 allert");
        args->setPoi(args, "context", self);
        args->setInt(args, "optionMax", 0);
        args->setInt(args, "dataIndex", 5);

        gui_set1 = New_gui2_set(args);

        args->dinit(args);
    }

    gui_main = gui_home;
    self->subObjectList[0] = gui_main;
		self->subObjectList[1] = key_0;
		self->subObjectList[2] = key_1;
		self->subObjectList[3] = key_2;

    //link guis
    gui_home->next_gui[0] = gui_observer;
    gui_home->next_gui[1] = gui_setAlert;

    gui_observer->next_gui[0] = gui_watch0;
    gui_observer->next_gui[1] = gui_watch1;
    gui_observer->next_gui[2] = gui_watch2;
    gui_observer->next_gui[3] = gui_watch3;

    gui_setAlert->next_gui[0] = gui_set0;
    gui_setAlert->next_gui[1] = gui_set1;

    /* override */
}

server_t *New_server_panelTemp(args_t *args)
{
    server_t *self = New_server(args);
    self->init = init_panelTemp;
    self->init(self, args);
    return self;
}