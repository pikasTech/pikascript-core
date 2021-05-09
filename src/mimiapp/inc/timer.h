#ifndef _mimi_timer__H
#define _mimi_timer__H
#include "VM_memory.h"
#include "callback.h"
typedef struct Class_timer mTimer_t;

struct Class_timer
{
    DMEM *mem;
    void (*dinit)(mTimer_t *timer);

    /* attribute */
    unsigned char isEnable;
    int time_ms;
    int time_begin;
    int time_now;
    void *context;

    /* operation */
    void (*enable)(mTimer_t *timer);
    void (*disable)(mTimer_t *timer);
    void (*set)(mTimer_t *timer, int times_ms);
    void (*update)(mTimer_t *timer,int time_now_ms);
    

    /* object */
    callback_t *callback;

    /* need override */

    void (*_handler)(callback_t *callback);
};

mTimer_t *Class_timer_init(void (*_handler)(callback_t *callback));

#endif