#include "PORT_inductor.h"
#include "inductor.h"
#ifdef LINUX
#include <stdio.h>
#endif
#ifndef LINUX
#include "main.h"
#endif

static int PORT_stateUpdate(void)
{
#ifdef LINUX
    return 1;
#endif
#ifndef LINUX
		return !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
#endif
}

inductor_t *Class_inductor_init_PORT(inductor_t *induc)
{
    induc->PORT_stateUpdate = PORT_stateUpdate;
    return induc;
}