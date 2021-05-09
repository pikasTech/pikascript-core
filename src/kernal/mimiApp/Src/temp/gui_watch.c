#include "dataList.h"
#include "dataMemory.h"
#include "gui2.h"
#include "server.h"
#include "gui_3key.h"

static void refrash_optionPointer_void(gui2_t *self)
{
}

static void refresh_periodic_watch(gui2_t *self)
{
    server_t *panel = self->context;
    {
        char buff[32];
        sprintf(buff, "var: %1.4d",*(int*)(panel->subAttributeList[self->dataIndex]));
        self->showLine(self, 3, 1, buff);
    }
}

static void init_watch(gui2_t *self, list_t *args)
{
    /* attrivute */

    /* operation */
    self->_refresh_periodic = refresh_periodic_watch;
    self->refrash_optionPointer = refrash_optionPointer_void;

    /* object */

    /* override */
}

gui2_t *New_gui2_watch(list_t *args)
{
    gui2_t *self = New_gui2_3key(args);
    self->init = init_watch;
    self->init(self, args);
    return self;
}