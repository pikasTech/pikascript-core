#include "SH_sgui.h"
#include "VM_gui.h"
#include "VM_terminal.h"
#include "market_case1.h"
#include "mimiPort_market.h"
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
#include "mimiVM_core.h"
#include "panel_default.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

extern sh_t *PubSh;
terminal_t *terminal;
sgui_t *sgui;
market_case1_t *market;

static void callBack_char_linux_cli(terminal_t *terminal, char ch);

// the detector of shell luancher, which can add info befor the strout
static void *detector_sh_linux(void *(*fun_d)(char *, char *, void *(fun)(int, char **)), char *CMD, char *StartStr, void *(fun)(int argc, char **argv))
{
    DMEM *memOut;
    DMEM *memAdd;
    memAdd = DynMemGet(sizeof(char) * 256);
    char *strAdd = memAdd->addr;
    strAdd[0] = 0;
    memOut = (DMEM *)fun_d(CMD, StartStr, fun);
    if (NULL != memOut)
    {
        strPrint(strAdd, (char *)(memOut->addr));
        DynMemPut(memOut);
    }
    return memAdd;
}

static void callBack_char_linux_gui(terminal_t *terminal, char ch)
{
    gui_t *gui_main = market->panel->gui_main;
    if (3 == ch)
    {
        terminal->callBack_char = callBack_char_linux_cli;
        PubSh->cmd(PubSh, "to com1 gui exited.");
    }
    if ('j' == ch)
    {
        gui_main->up(gui_main);
    }
    if ('k' == ch)
    {
        gui_main->down(gui_main);
    }
    if ('h' == ch)
    {
        gui_main->back(gui_main);
    }
    if ('l' == ch)
    {
        gui_main->enter(gui_main);
    }
}

static void *sh_sgui_main_linux(int argc, char **argv)
{
    DMEM *strout_mem = DynMemGet(sizeof(char) * 256);
    char *strOut = strout_mem->addr;
    strOut[0] = 0;
    terminal->callBack_char = callBack_char_linux_gui;
    market->panel->gui_main->refresh(market->panel->gui_main);
    return (void *)strout_mem;
}

static void callBack_char_linux_cli(terminal_t *terminal, char ch)
{
    if (3 == ch)
    {
        terminal->PORT_VM_terminal_exit();
    }

    if ('~' == ch)
    {
        ch = '\r';
    }

    if ('\r' == ch)
    {
        terminal->isClearLastLine = 0;
    }

    *VM_com_RX_Char_pget(&VM.com1) = ch;
    VM_IT(VM_IT_COM1);

    if (terminal->isClearLastLine)
    {
        system("tput cuu1");
        system("tput el");
    }

    if ('\r' != ch)
    {
        char buff[512] = {0};
        sprintf(buff, "echo \"myShell@Linux >%s\r\"", VM.com1.RX_buff);
        system(buff);
        terminal->isClearLastLine = 1;
    }
}

sh_t *Shell_Core_init_linux(void)
{
    PubSh = ShellCore_init();
    PubSh->detector = detector_sh_linux;
    return PubSh;
}

terminal_t *VM_terminal_init_linux(void)
{
    terminal_t *terminal = VM_terminal_init();
    terminal->PORT_VM_terminal_exit = PORT_VM_terminal_exit_linux;
    terminal->PORT_VM_terminal_getChar = PORT_VM_terminal_getChar_linux;
    terminal->callBack_char = callBack_char_linux_cli;
    return terminal;
}

sgui_t *SH_sgui_init_linux(void)
{
    sgui_t *sgui = SH_sgui_init();
    sgui->sh_main = sh_sgui_main_linux;
    return sgui;
}

int main(int argc, char **argv)
{
    long long i_ms = 0;
    system(STTY_US TTY_PATH);
    system("tput civis");

    PubSh = Shell_Core_init_linux();
    terminal = VM_terminal_init_linux();
    market = Class_market_case1_init();
    sgui = SH_sgui_init_linux();

    PubSh->addMap(PubSh, "gui", sgui->sh_main);

    DynMemPut((DMEM *)PubSh->cmd(PubSh, (char *)"iot init"));
    DynMemPut((DMEM *)PubSh->cmd(PubSh, (char *)"uart init"));

    printf("[ Welcom to mimiSH! ]\r\n");

    while (1)
    {
        usleep(5000);
        i_ms += 5;

        if (0 == i_ms % 50)
        {
            // refresh gui pernal
            market->update(market, i_ms);
        }

        if (0 == i_ms % 50)
        {
            VM_main();
        }

        if (0 == i_ms % 2)
        {
            VM_vIT();
        }

        if (0 == i_ms % 50)
        {
            terminal->update(terminal);
        }
    }
}
