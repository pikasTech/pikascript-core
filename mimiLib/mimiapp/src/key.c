#include "key.h"
#include "VM_gui.h"
#include "VM_key.h"
#include "VM_memory.h"
#include "mimiPort.h"
#include "mimiSH_config.h"

vkey_t *key_up;
vkey_t *key_down;
vkey_t *key_enter;
vkey_t *key_back;

void callback_key_up(vkey_t *key, void *arg)
{
	DynMemPut((DMEM *)(PubSh->cmd(PubSh, "to com1 key_up is pressed!")));
	//	Pub_gui->up(Pub_gui);
};

void callback_key_down(vkey_t *key, void *arg)
{
	DynMemPut((DMEM *)(PubSh->cmd(PubSh, "to com1 key_down is pressed!")));
	//	Pub_gui->down(Pub_gui);
};

void callback_key_enter(vkey_t *key, void *arg)
{
	DynMemPut((DMEM *)(PubSh->cmd(PubSh, "to com1 key_enter is pressed!")));
	//	Pub_gui->enter(Pub_gui);
};

void callback_key_back(vkey_t *key, void *arg)
{
	DynMemPut((DMEM *)(PubSh->cmd(PubSh, "to com1 key_back is pressed!")));
	//	Pub_gui->back(Pub_gui);
};

static void key_up_init(void)
{
	key_up = VM_key_init();
	key_up->PORT_pressGet = PORT_pressGet_key_up;
	key_up->callback = callback_key_up;
}

static void key_down_init(void)
{
	key_down = VM_key_init();
	key_down->PORT_pressGet = PORT_pressGet_key_down;
	key_down->callback = callback_key_down;
}

static void key_enter_init(void)
{
	key_enter = VM_key_init();
	key_enter->PORT_pressGet = PORT_pressGet_key_enter;
	key_enter->callback = callback_key_enter;
}

static void key_back_init(void)
{
	key_back = VM_key_init();
	key_back->PORT_pressGet = PORT_pressGet_key_back;
	key_back->callback = callback_key_back;
}

void key_init(void)
{
	key_up_init();
	key_down_init();
	key_enter_init();
	key_back_init();
}