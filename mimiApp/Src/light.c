#include "light.h"
#include "dataMemory.h"

static void deinit(light_t *light)
{
    DynMemPut(light->mem);
}

static void openLight(light_t *light)
{
    light->PORT_setLight(1);
}

static void close(light_t *light)
{
    light->PORT_setLight(0);
}

static void PORT_setLight(int state)
{
    // rewrite here in port file
}
light_t *Class_light_init(void)
{
    DMEM *mem = DynMemGet(sizeof(light_t));
    light_t *light = mem->addr;
    light->mem = mem;
    light->deinit = deinit;

    light->open = openLight;
    light->close = close;
    light->PORT_setLight = PORT_setLight;
    return light;
}
