#include "VM_key.h"
#include "dataMemory.h"

static void update_port(vkey_t *key, int systime_ms)
{
	int isPressNow = key->PORT_pressGet();

	if(systime_ms < ( key->callBack_time_ms + key->callBack_delay_ms ))
	{
		return;
	}
	
	if(isPressNow)
	{
		key->set(key,1);
	}
}

static void update_callBack(vkey_t *key, int systime_ms)
{
	if(systime_ms < ( key->callBack_time_ms + key->callBack_delay_ms ))
	{
		return;
	}
	
  if (key->get(key))
  {
		key->callback(key, key->callBack_arg);
		key->set(key, 0);
		key->callBack_time_ms = systime_ms;
	}
}

// callback need to be write in user code.
static void callback_default(vkey_t *key, void *arg)
{
    // do something about callback
}
static int get(vkey_t *key)
{
    return key->isPress;
}
static void set(vkey_t *key, int val)
{
    key->isPress = val;
}
// get proess by hardware, need to be rewrite in user code.
static int pressGet_default(void)
{
    return 1;
}

void deinit(vkey_t *key)
{
    DynMemPut(key->mem);
}

vkey_t *VM_key_init(void)
{
    DMEM *mem = DynMemGet(sizeof(vkey_t));
    vkey_t *key = (vkey_t *)mem->addr;
    key->mem = mem;
    key->isPress = 0;
    key->deinit = deinit;
    key->update_port = update_port;
    key->update_callBack = update_callBack;
    key->callback = callback_default;
		key->callBack_delay_ms = 200;
    key->callBack_arg = NULL;
		key->callBack_time_ms = 0;
		key->get = get;
    key->set = set;
    key->PORT_pressGet = pressGet_default;
		
		key->context = key;
    return key;
}
