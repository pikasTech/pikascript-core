#include "dataMemory.h"
#include "VM_smoke.h"

smoke_t *Pub_VM_smoke;

static void deinit(struct VM_smoke *smoke)
{
	DynMemPut(smoke->mem);
}

static float get_val(struct VM_smoke *smoke)
{
	return smoke->val;
}

static void update(struct VM_smoke *smoke)
{
	float val = 0;
	unsigned short int ADC_val = 0;
	ADC_val = smoke->PORT_get_val();
	val = ADC_val;
	smoke->val = val;
}

unsigned short int PORT_get_val_default(void)
{
	// rewrite the PORT funtion in mimiPort.c
	return 0;
}

smoke_t *VM_smoke_init(void)
{
	DMEM *mem = DynMemGet(sizeof(smoke_t));
	smoke_t *smoke = (smoke_t *)mem->addr;
	smoke->mem = mem;
	smoke->deinit = deinit;
	smoke->get_val = get_val;
	smoke->update = update;
	smoke->PORT_get_val = PORT_get_val_default;

	return smoke;
}