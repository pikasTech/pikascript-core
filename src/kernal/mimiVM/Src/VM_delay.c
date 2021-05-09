#include "VM_delay.h"
#include "dataMemory.h"
#include "mimiVM_core.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef LINUX
#include "main.h"
#endif

struct VM_delay *Pub_VM_Delay;

void delay(struct VM_delay *vd, int delay_ms)
{
#ifdef LINUX
  char buffer[256] = {0};
  sprintf(buffer, "[sys]:\r\ndelay : %d\r\n", delay_ms);

  VM_UART_Transmit(&VM.com1, buffer);
// printf(",delay_ms);
#endif
#ifndef LINUX
  HAL_Delay(delay_ms);
#endif
}

static void deinit(struct VM_delay *vd)
{
  DynMemPut(vd->mem);
}

struct VM_delay *VM_delay_init(void)
{
  DMEM *mem = DynMemGet(sizeof(struct VM_delay));
  struct VM_delay *vd = (struct VM_delay *)(mem->addr);
  vd->mem = mem;
  vd->delay = delay;
  vd->deinit = deinit;
  vd->context = vd;
  return vd;
}
