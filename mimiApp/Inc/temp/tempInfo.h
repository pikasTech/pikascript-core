#ifndef _tempInfo__H
#define _tempInfo__H
#include "dataMemory.h"
#include "dataArgs.h"
typedef struct Class_tempInfo tempInfo_t;
struct Class_tempInfo
{
    /* attribute */
    DMEM *mem;
    void *context;
    int temp1;
    int temp2;
    int hum1;
    int hum2;
	
		int addr_temp1;
		int addr_temp2;
		int addr_hum1;
		int addr_hum2;

    /* operation */
    void (*deinit)(tempInfo_t *self);
    void (*init)(tempInfo_t *self, Args *args);
    void (*set)(tempInfo_t *self, char *varName, int var);
    int (*get)(tempInfo_t *self, char *varName);

    /* virtual operation */

    /* object */
};

tempInfo_t *New_tempInfo(Args *args);
#endif