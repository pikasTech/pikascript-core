#include "mimiPort_lamp.h"
#include "dataMemory.h"
#include "light.h"
static void deinit(mimiPort_lamp_t *mimiPort_lamp)
{
    DynMemPut(mimiPort_lamp->mem);
}
mimiPort_lamp_t *Class_mimiPort_lamp_init(void)
{
    DMEM *mem = DynMemGet(sizeof(mimiPort_lamp_t));
    mimiPort_lamp_t *mimiPort_lamp = mem->addr;
    mimiPort_lamp->mem = mem;
    mimiPort_lamp->deinit = deinit;
    return mimiPort_lamp;
}

static void PORT_setLight(int state)
{
    if (0 == state)
    {
#ifdef LINUX

        printf("close the ligh\r\n");
#endif
#ifndef LINUX
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
#endif
        return;
    }

    if (1 == state)
    {
#ifdef LINUX
        printf("open the light\r\n");
#endif
#ifndef LINUX
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
#endif
        return;
    }

    printf("[error] iligal operation to setLight!\r\n");
    return;
}

light_t *Class_light_init_PORT(void)
{
    light_t *light = Class_light_init();
    light->PORT_setLight = PORT_setLight;

    return light;
}