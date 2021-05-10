#ifndef _mimiPort_lamp__H
#define _mimiPort_lamp__H
#include "dataMemory.h"
#include "light.h"
typedef struct Class_mimiPort_lamp mimiPort_lamp_t;
struct Class_mimiPort_lamp
{
    DMEM *mem;
    void (*dinit)(mimiPort_lamp_t *mimiPort_lamp);
};
mimiPort_lamp_t *Class_mimiPort_lamp_init(void);

light_t *Class_light_init_PORT(void);

#endif