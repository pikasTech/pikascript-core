#ifndef __VM_SMOKE_H
#define __VM_SMOKE_H

#include "dataMemory.h"
typedef struct VM_smoke
{
	DMEM *mem;
	void (*deinit)(struct VM_smoke *);
	float val;
	void (*update)(struct VM_smoke *);
	float (*get_val)(struct VM_smoke *);
	unsigned short int (*PORT_get_val)(void);
}smoke_t;

smoke_t *VM_smoke_init(void);

#endif
