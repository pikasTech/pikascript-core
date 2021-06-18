#include "PORT_com.h"
#include "VM_iot.h"
#include "allert.h"
#include "dataMemory.h"
#include "gui2.h"
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "server.h"
#include "shApp_set.h"
#include "tempInfo.h"
#include "allertInfo.h"
#include "temp_panel.h"
#include <stdlib.h>

extern sh_t *PubSh;
extern iot_t *iot;
server_t *tempM;

static void update(server_t *self, int systime)
{
	tempInfo_t *tempInfo = self->subObjectList[0];
	server_t *allert = self->subObjectList[2];
	server_t *panel = self->subObjectList[3];

	// return if lamp is not enable
	if (0 == self->isEnable)
	{
		return;
	}

	allert->update(allert, systime);
	panel->update(panel, systime);

	if (0 == systime % 3000)
	{
		char str[128] = {0};
		sprintf(str, "[info]: temp1:%d, temp2:%d, hum1:%d, hum2:%d\r\n", tempInfo->get(tempInfo, "temp1"), tempInfo->get(tempInfo, "temp2"), tempInfo->get(tempInfo, "hum1"), tempInfo->get(tempInfo, "hum2"));
		PORT_send_to_com(1, str);
	}

	if (0 == (systime - 1000) % 4000)
	{
		iot->data_upload_int(iot, tempInfo->addr_temp1, tempInfo->temp1);
	}

	if (0 == (systime - 2000) % 4000)
	{
		iot->data_upload_int(iot, tempInfo->addr_temp2, tempInfo->temp2);
	}

	if (0 == (systime - 3000) % 4000)
	{
		iot->data_upload_int(iot, tempInfo->addr_hum1, tempInfo->hum1);
	}

	if (0 == (systime - 4000) % 4000)
	{
		iot->data_upload_int(iot, tempInfo->addr_hum2, tempInfo->hum2);
	}
}

static void init_tempM(server_t *self, args_t *args)
{
	/* attrivute */

	/* operation */
	self->update = update;

	/* object */
	tempInfo_t *tempInfo = New_tempInfo(NULL);
	shApp_set_t *shSet = New_shApp_set(NULL);
	PubSh->addMap(PubSh, "set ", shSet->sh_main);
	server_t *allert = NULL;
	server_t *panel = NULL;
	
	{
		args_t *args = New_args(NULL);
		args->pushPointerWithName(args, "context", tempInfo);
		allert = New_server_allert(args);
		args->dinit(args);
	}
	{
		args_t *args = New_args(NULL);
		args->pushInt64WithName(args, "isEnable", 0);
		args->pushPointerWithName(args, "context", self);
		panel = New_server_panelTemp(args);
		args->dinit(args);
	}

	allertInfo_t *allertInfo = allert->subObjectList[0];
	// link
	self->subObjectList[0] = tempInfo;
	self->subObjectList[1] = shSet;
	self->subObjectList[2] = allert;
	self->subObjectList[3] = panel;

	panel->subAttributeList[0] = &(tempInfo->temp1);
	panel->subAttributeList[1] = &(tempInfo->temp2);
	panel->subAttributeList[2] = &(tempInfo->hum1);
	panel->subAttributeList[3] = &(tempInfo->hum2);
	panel->subAttributeList[4] = &(allertInfo->allert_temp1);
	panel->subAttributeList[5] = &(allertInfo->allert_temp2);

	/* override */
}

server_t *New_server_tempM(args_t *args)
{
	server_t *self = New_server(args);
	self->init = init_tempM;
	self->init(self, args);
	return self;
}