#include "PORT_com.h"
#include "PORT_gpio.h"
#include "dataMemory.h"
#include "allertInfo.h"
#include "server.h"
#include "tempInfo.h"

static void update(server_t *self, int systime)
{
    tempInfo_t *tempInfo = self->context;
    allertInfo_t *allertInfo = self->subObjectList[0];
    // return if lamp is not enable
    if (0 == self->isEnable)
    {
        return;
    }

    if (0 != systime % 300)
    {
        return;
    }

    if (tempInfo->temp1 >= allertInfo->allert_temp1)
    {
        PORT_send_to_com(1, "allert!!\r\n");
        PORT_gpio_set(PORT_GPIO_GROUP_A, 4, 1);
        return;
    }

    if (tempInfo->temp2 >= allertInfo->allert_temp2)
    {
        PORT_send_to_com(1, "allert!!\r\n");
        PORT_gpio_set(PORT_GPIO_GROUP_A, 4, 1);
        return;
    }

    PORT_gpio_set(PORT_GPIO_GROUP_A, 4, 0);
    return;
}

static void init_allert(server_t *self, args_t *args)
{
    /* attrivute */

    /* operation */
    self->update = update;

    /* object */
    self->subObjectList[0] = New_allertInfo(NULL);

    /* override */
}

server_t *New_server_allert(args_t *args)
{
    server_t *self = New_server(args);
    self->init = init_allert;
    self->init(self, args);
    return self;
}