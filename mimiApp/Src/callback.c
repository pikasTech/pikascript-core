#include "callback.h"
#include "dataMemory.h"


#define ERROR_HANDLE_DO_NOT_BE_OVERRIDE 1

static void deinit(callback_t *callback)
{
    DynMemPut(callback->mem);
}

static void update(callback_t *callback)
{
    // check the flag and callback the handle if the flag is set by user

    // return if callback not enable
    if (0 == callback->enable)
    {
        return;
    }

    // return if flag is not be set
    if (0 == callback->flag)
    {
        return;
    }

    callback->_handler(callback);
    // set flag to 0 to run handler only once when flag is set to 1
    callback->flag = 0;
    return;
}

static void _handleNeedOverride(callback_t *callback)
{
    // override in user code
    callback->error = ERROR_HANDLE_DO_NOT_BE_OVERRIDE;
    // disable the callback
    callback->enable = 0;
}

static void enable(callback_t *callback)
{
    callback->isEnable = 1;
}

static void disable(callback_t *callback)
{
    callback->isEnable = 0;
}

static void trigger(callback_t *callback)
{
    // set the flag to trigger the handle
    callback->flag = 1;
}

callback_t *Class_callback_init(void)
{
    /* base operation and attribute */
    DMEM *mem = DynMemGet(sizeof(callback_t));
    callback_t *callback = mem->addr;
    callback->mem = mem;
    callback->dinit = deinit;

    /* attribute */
    callback->flag = 0;
    callback->error = 0;
    callback->isEnable = 1;
    callback->context = callback;

    /* opration */
    callback->update = update;
    callback->enable = enable;
    callback->disable = disable;
    callback->trigger = trigger;

    /* opration need to override */
    callback->_handler = _handleNeedOverride;

    return callback;
}