#include "dataArgs.h"
#include "dataMemory.h"
#include "gui2.h"
#include "server.h"

static void refrash_optionPointer_void(gui2_t *self)
{
}

static void refresh_data_set(gui2_t *self)
{
    server_t *panel = self->context;
    {
        char buff[32];
        sprintf(buff, "var: %1.4d", *(int *)(panel->subAttributeList[self->dataIndex]));
        self->showLine(self, 3, 1, buff);
    }
}

void varSet_add(gui2_t *self)
{
    server_t *panel = self->context;
    int *var_p = (int *)(panel->subAttributeList[self->dataIndex]);
    *var_p += 1;
    refresh_data_set(self);
}

void varSet_dec(gui2_t *self)
{
    server_t *panel = self->context;
    int *var_p = (int *)(panel->subAttributeList[self->dataIndex]);
    if (*var_p <= 0)
    {
        return;
    }
    *var_p -= 1;
    refresh_data_set(self);
}

static void varSet_ok(gui2_t *self)
{
    self->back_callBack(self);
}

static void refresh(gui2_t *self)
{
    self->clear(self);
    self->showLine(self, 6, 0, "----------------------");
    self->showLine(self, 7, 0, "|ADD");
    self->showLine(self, 7, 0 + 5, "|   DEC ");
    self->showLine(self, 7, 0 + 5 + 5 + 5, "| OK |");

    self->refrash_optionPointer(self);
    self->refrash_optionStr(self);
    self->refresh_title(self);
    refresh_data_set(self);
}

static void init_set(gui2_t *self, args_t *args)
{
    /* attrivute */

    /* operation */
    self->refresh = refresh;
    self->refrash_optionPointer = refrash_optionPointer_void;
    self->back = varSet_add;
    self->enter = varSet_ok;
    self->down = varSet_dec;

    /* object */

    /* override */
}

gui2_t *New_gui2_set(args_t *args)
{
    gui2_t *self = New_gui2(args);
    self->init = init_set;
    self->init(self, args);
    return self;
}