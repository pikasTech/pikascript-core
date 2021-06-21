#include "gui2.h"
#include "PORT_gui.h"
#include "dataMemory.h"
#include "dataString.h"
#include "server.h"

static void deinit(gui2_t *self)
{
    DynMemPut(self->mem);
}

static void up(gui2_t *self)
{
    if (self->option_pointer > 0)
    {
        self->option_pointer--;
        self->refrash_optionPointer(self);

        return;
    }

    self->option_pointer = self->option_pointer_max;
    self->refrash_optionPointer(self);

    return;
}

static void down(gui2_t *self)
{
    if (self->option_pointer < self->option_pointer_max)
    {
        self->option_pointer++;
        self->refrash_optionPointer(self);
        return;
    }

    self->option_pointer = 0;
    self->refrash_optionPointer(self);

    return;
}

static void refrash_optionPointer(gui2_t *self)
{
    int pointer_line_new = self->option_pointer * 1 + 1;
    self->showLine(self, self->option_pointer_line, 1, "  ");
    self->showLine(self, pointer_line_new, 1, "->");
    self->option_pointer_line = pointer_line_new;
}

static void enter(gui2_t *self)
{
    self->_option_callBack[self->option_pointer](self);
}

static void back(gui2_t *self)
{
    self->back_callBack(self);
}

static void refresh(gui2_t *self)
{
    self->clear(self);
    self->showLine(self, 6, 0, "----------------------");
    self->showLine(self, 7, 0, "|Back");
    self->showLine(self, 7, 0 + 5, "| Up");
    self->showLine(self, 7, 0 + 5 + 5, "|Down");
    self->showLine(self, 7, 0 + 5 + 5 + 5, "|Enter|");

    self->refrash_optionPointer(self);
    self->refrash_optionStr(self);
    self->refresh_title(self);
    self->_refresh_periodic(self);
}

static void refrash_optionStr(gui2_t *self)
{
    int option_str_line;
    for (int i = 0; i < 5; i++)
    {
        option_str_line = i + 1;
        self->showLine(self, option_str_line, 5, self->option_str[i]);
    }
}

static void refresh_title(gui2_t *self)
{
    self->showLine(self, 0, 1, self->title);
}

static void _callBack_default(gui2_t *self)
{
    //do nothing
}

static void showString(gui2_t *self, int x, int y, char *string)
{
    PORT_gui_showXY(x, y, string);
}

static void clear(gui2_t *self)
{
    PORT_gui_clear();
}

static void change_gui(gui2_t *self, gui2_t *gui_new)
{
    // log the last_gui to back
    gui_new->last_gui = self;
    self = gui_new;
    // refresh
    self->refresh(self);
}

static void back_callBack(gui2_t *self)
{
    server_t *panel = self->context;
    gui2_t *guiNow = panel->subObjectList[0];
    gui2_t *guiLast = guiNow->last_gui;

    panel->subObjectList[0] = guiLast;

    gui2_t *gui_main = panel->subObjectList[0];
    gui_main->refresh(gui_main);
}

static void guiChange_callBack(gui2_t *self)
{
    // get the context
    server_t *panel = self->context;

    // log gui Now
    gui2_t *guiNow = panel->subObjectList[0];
    gui2_t *guiNext = guiNow->next_gui[guiNow->option_pointer];
    // change gui_main
    panel->subObjectList[0] = guiNext;
    gui2_t *gui_main = panel->subObjectList[0];
    gui_main->refresh(gui_main);
    gui_main->last_gui = guiNow;
}

static void _refresh_periodic_deault(gui2_t *self)
{
    // rewrite here if need periodic refresh.
}

static void setTitle(gui2_t *self, char *title)
{
    for (int i = 0; i < 32; i++)
    {
        self->title[i] = 0;
    }
    strPrint(self->title, title);
}

static void showLine(gui2_t *gui, int line, int cols, char *str)
{
    // the map of line to x,y is for linux, rewrite it in other platform, shuch as lcd.
#ifndef LINUX
    int x = 0;
    int y = 0;
    x = cols * 6;
    y = line * 15 + 10;
    gui->showString(gui, x, y, str);
#endif
#ifdef LINUX
    gui->showString(gui, cols, line, str);
#endif
}

static void argHandle_title(gui2_t *self,
                            args_t *args,
                            char *argName)
{
    char *title = args->getStr(args, argName);
    self->setTitle(self, title);
}

static void argHandle_context(gui2_t *self,
                              args_t *args,
                              char *argName)
{
    self->context = args->getPoi(args, argName);
}

static void argHandle_optionMax(gui2_t *self,
                                args_t *args,
                                char *argName)
{
    self->option_pointer_max = args->getInt(args, argName);
}

static void argHandle_dataIndex(gui2_t *self,
                                args_t *args,
                                char *argName)
{
    self->dataIndex = args->getInt(args, argName);
}

static void argHandle_optionStr0(gui2_t *self,
                                 args_t *args,
                                 char *argName)
{
    char *str = args->getStr(args, argName);
    strPrint(self->option_str[0], str);
}

static void argHandle_optionStr1(gui2_t *self,
                                 args_t *args,
                                 char *argName)
{
    char *str = args->getStr(args, argName);
    strPrint(self->option_str[1], str);
}

static void argHandle_optionStr2(gui2_t *self,
                                 args_t *args,
                                 char *argName)
{
    char *str = args->getStr(args, argName);
    strPrint(self->option_str[2], str);
}

static void argHandle_optionStr3(gui2_t *self,
                                 args_t *args,
                                 char *argName)
{
    char *str = args->getStr(args, argName);
    strPrint(self->option_str[3], str);
}

static void argHandle_optionStr4(gui2_t *self,
                                 args_t *args,
                                 char *argName)
{
    char *str = args->getStr(args, argName);
    strPrint(self->option_str[4], str);
}

static void argHandle_optionCallBack0(gui2_t *self,
                                      args_t *args,
                                      char *argName)
{
    char *str = args->getStr(args, argName);
    if (0 == strcmp("guiChange", str))
    {
        self->_option_callBack[0] = self->guiChange_callBack;
    }
}

static void argHandle_optionCallBack1(gui2_t *self,
                                      args_t *args,
                                      char *argName)
{
    char *str = args->getStr(args, argName);
    if (0 == strcmp("guiChange", str))
    {
        self->_option_callBack[1] = self->guiChange_callBack;
    }
}

static void argHandle_optionCallBack2(gui2_t *self,
                                      args_t *args,
                                      char *argName)
{
    char *str = args->getStr(args, argName);
    if (0 == strcmp("guiChange", str))
    {
        self->_option_callBack[2] = self->guiChange_callBack;
    }
}

static void argHandle_optionCallBack3(gui2_t *self,
                                      args_t *args,
                                      char *argName)
{
    char *str = args->getStr(args, argName);
    if (0 == strcmp("guiChange", str))
    {
        self->_option_callBack[3] = self->guiChange_callBack;
    }
}

static void argHandle_optionCallBack4(gui2_t *self,
                                      args_t *args,
                                      char *argName)
{
    char *str = args->getStr(args, argName);
    if (0 == strcmp("guiChange", str))
    {
        self->_option_callBack[4] = self->guiChange_callBack;
    }
}

static void argHandle(gui2_t *self,
                      args_t *args,
                      char *argName,
                      void (*handle)(gui2_t *self,
                                     args_t *args,
                                     char *argName))
{
    if (-1 == args->getIndexByName(args, argName))
    {
        return;
    }
    handle(self, args, argName);
}

static void init(gui2_t *self, args_t *args)
{
    /* attrivute */
    self->title[0] = 0;
    self->context = self;
    self->last_gui = self;
    self->generalData = NULL;
    self->option_pointer = 0;
    self->option_pointer_max = 4;
    self->option_pointer_line = 0;
    self->dataIndex = 0;
    for (int i = 0; i < 5; i++)
    {
        self->option_str[i][0] = 0;
        self->next_gui[i] = self;
    }

    /* operation */
    self->dinit = deinit;
    self->up = up;
    self->down = down;
    self->back = back;
    self->enter = enter;
    self->clear = clear;
    self->refresh = refresh;
    self->setTitle = setTitle;
    self->change_gui = change_gui;
    self->showString = showString;
    self->showLine = showLine;
    self->back_callBack = back_callBack;
    self->refresh_title = refresh_title;
    self->refrash_optionStr = refrash_optionStr;
    self->refrash_optionPointer = refrash_optionPointer;
    self->argHandle = argHandle;
    self->guiChange_callBack = guiChange_callBack;

    /* object */

    /* override */
    self->_refresh_periodic = _refresh_periodic_deault;
    for (int i = 0; i < 5; i++)
    {
        self->_option_callBack[i] = _callBack_default;
    }

    /* args */
    if (NULL == args)
    {
        return;
    }
    self->argHandle(self,
                    args,
                    "title",
                    argHandle_title);
    self->argHandle(self,
                    args,
                    "context",
                    argHandle_context);
    self->argHandle(self,
                    args,
                    "optionMax",
                    argHandle_optionMax);
    self->argHandle(self,
                    args,
                    "optionStr0",
                    argHandle_optionStr0);
    self->argHandle(self,
                    args,
                    "optionStr1",
                    argHandle_optionStr1);
    self->argHandle(self,
                    args,
                    "optionStr2",
                    argHandle_optionStr2);
    self->argHandle(self,
                    args,
                    "optionStr3",
                    argHandle_optionStr3);
    self->argHandle(self,
                    args,
                    "optionStr4",
                    argHandle_optionStr4);
    self->argHandle(self,
                    args,
                    "optionCallBack0",
                    argHandle_optionCallBack0);
    self->argHandle(self,
                    args,
                    "optionCallBack1",
                    argHandle_optionCallBack1);
    self->argHandle(self,
                    args,
                    "optionCallBack2",
                    argHandle_optionCallBack2);
    self->argHandle(self,
                    args,
                    "optionCallBack3",
                    argHandle_optionCallBack3);
    self->argHandle(self,
                    args,
                    "optionCallBack4",
                    argHandle_optionCallBack4);
    self->argHandle(self,
                    args,
                    "dataIndex",
                    argHandle_dataIndex);
}

gui2_t *New_gui2(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(gui2_t));
    gui2_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}