#include "gui2.h"
#include "dataMemory.h"
#include "dataArgs.h"

static void refresh(gui2_t *self)
{
    self->clear(self);
    self->showLine(self, 6, 0, "----------------------");
    self->showLine(self, 7, 0, "|Back");
    self->showLine(self, 7, 0 + 5, "|   Next   ");
    self->showLine(self, 7, 0 + 5 + 5 + 5, "|Enter|");

    self->refrash_optionPointer(self);
    self->refrash_optionStr(self);
    self->refresh_title(self);
    self->_refresh_periodic(self);
}

static void init_3key(gui2_t *self, Args *args)
{
    /* attrivute */

    /* operation */
    self->refresh = refresh;

    /* object */

    /* override */
}

gui2_t *New_gui2_3key(Args *args)
{
    gui2_t *self = New_gui2(args);
    self->init = init_3key;
    self->init(self, args);
    return self;
}