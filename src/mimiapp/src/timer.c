#include "VM_memory.h"
#include "timer.h"
#include "callback.h"

static void deinit(mTimer_t *timer)
{
    DynMemPut(timer->mem);
    timer->callback->dinit(timer->callback);
}

static void enable(mTimer_t *timer)
{
    timer->isEnable = 1;
    timer->time_begin = timer->time_now;
}

static void disable(mTimer_t *timer)
{
    timer->isEnable = 0;
}

static void set(mTimer_t *timer, int times_ms)
{
    timer->time_ms = times_ms;
}

static void update(mTimer_t *timer, int time_now_ms)
{
    callback_t *callback = timer->callback;
    timer->time_now = time_now_ms;
    callback->update(callback);

    // return if not enable
    if (0 == timer->isEnable)
    {
        return;
    }

    // return if do not reach timer_triggle
    int time_triggle = timer->time_begin + timer->time_ms;
    if (timer->time_now < time_triggle)
    {
        return;
    }

    // trigger the callback
    callback->trigger(callback);

    timer->disable(timer);
}

mTimer_t *Class_timer_init(void (*_handler)(callback_t *callback))
{
    DMEM *mem = DynMemGet(sizeof(mTimer_t));
    mTimer_t *timer = mem->addr;
    timer->mem = mem;
    timer->dinit = deinit;

    /* attribute */
    timer->isEnable = 0;
    timer->time_ms = 0;
    timer->time_now = 0;
    timer->context = timer;

    /* operation */
    timer->enable = enable;
    timer->disable = disable;
    timer->set = set;
    timer->update = update;

    /* object */
    timer->callback = Class_callback_init();
    timer->callback->context = timer;

    /* override */
    timer->callback->_handler = _handler;

    return timer;
}