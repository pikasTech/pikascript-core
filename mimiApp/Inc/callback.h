#ifndef _callback__H
#define _callback__H
#include "dataMemory.h"
typedef struct Class_callback callback_t;
struct Class_callback
{
    DMEM *mem;
    void (*deinit)(callback_t *callback);
    void (*update)(callback_t *callback);


    void (*enable)(callback_t *callback);
    void (*disable)(callback_t *callback);
    
    void (*trigger)(callback_t *callback);

    unsigned char isEnable;
    unsigned char flag;
    unsigned char error;
    void *context;

    // start with "_" means should no be called by user
    void (*_handler)(callback_t *callback);
    
};
callback_t *Class_callback_init(void);
#endif