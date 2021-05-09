#ifndef __MIMI_PORT__H
#define __MIMI_PORT__H
#include "VM_gui.h"
#include "VM_key.h"
#include "VM_socket.h"
#include "VM_delay.h"

#define TTY_PATH "/dev/tty"
#define STTY_US "stty raw -echo -F "
#define STTY_DEF "stty -raw echo -F"

vkey_t *VM_key_init_PORT(int KEY_id);

VM_socket_t *VM_socket_init_PORT(int socket_ID);
struct VM_delay *VM_delay_init_PORT(void);
void PORT_VM_gui_clear(void);
void PORT_VM_gui_showString(int x, int y, char *string);
gui_t *VM_gui_init_PORT(void);
void	PORT_reboot(void);
void PORT_VM_terminal_exit_linux(void);
char PORT_VM_terminal_getChar_linux(void);
void PORT_reboot(void);
#endif