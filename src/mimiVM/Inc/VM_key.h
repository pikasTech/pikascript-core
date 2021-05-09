#ifndef __VM_KEY__H
#define __VM_KEY__H

#include "VM_key.h"
#include "VM_memory.h"

typedef struct VM_key vkey_t;

struct VM_key
{
    DMEM *mem;
    int isPress;
    void (*deinit)(vkey_t *key);
    void (*update_port)(vkey_t *, int systime_ms);
    void (*update_callBack)(vkey_t *, int systime_ms);
		int callBack_time_ms;
		int callBack_delay_ms;
    // callback need to be write in user code.
    void (*callback)(vkey_t *,void *arg);
    // callback_arg is the argument in callback, should be rewrite in user code
    void *callBack_arg;
    int (*get)(vkey_t *);
    void (*set)(vkey_t *, int val);
    // get proess by hardware, need to be rewrite in user code.
    int (*PORT_pressGet)(void);
	
		void *context;
};

vkey_t *VM_key_init(void);

#endif
