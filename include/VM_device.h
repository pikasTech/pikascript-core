#ifndef _VM_device__H
#define _VM_device__H
#include "VM_args.h"
#include "VM_memory.h"
typedef struct Class_device device_t;
struct Class_device
{
    /* attribute */
    DMEM *mem;
    void *context;

    /* operation */
    void (*dinit)(device_t *self);
    void (*init)(device_t *self, int argc, char **argv);
    int (*read)(device_t *self, args_t *args_in, args_t *args_out);
    float (*read_float)(device_t *self, args_t *args_in);
    int (*write)(device_t *self, args_t *args_in);
    int (*write_str)(device_t *self, char *str);
		int (*print)(device_t *self, char *str, float val);
	
    /* virtual operation */
    int (*_read_handle)(device_t *self, args_t *args_in, args_t *args_out);
    int (*_writeHandle)(device_t *self, args_t *args_in);

    /* object */
};

device_t *New_device(int argc, char **argv);
#endif