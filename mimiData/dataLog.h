#ifndef __DATA_LOG__H
#define __DATA_LOG__H
#include "dataMemory.h"


typedef struct dataLog
{
   DMEM *mem;

   DMEM *log_mem;
   char *log;
   int log_size;
   void (*print)(struct dataLog *loger, char *str);
   void (*deinit)(struct dataLog *loger);
} loger_t;

loger_t *dataLog_init(void);

#endif
