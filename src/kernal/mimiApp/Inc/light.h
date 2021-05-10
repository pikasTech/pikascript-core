#ifndef _light__H
#define _light__H
#include "dataMemory.h"
typedef struct Class_light light_t;
struct Class_light
{
    DMEM *mem;
    void (*dinit)(light_t *light);

    void (*open)(light_t *light);
    void (*close)(light_t *light);
    
    void (*PORT_setLight)(int state);
};
light_t *Class_light_init(void);
#endif