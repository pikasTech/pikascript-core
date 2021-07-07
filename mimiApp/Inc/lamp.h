#ifndef _lamp__H
#define _lamp__H
#include "VM_key.h"
#include "dataMemory.h"
#include "callback.h"
#include "inductor.h"
#include "light.h"
#include "panel_lamp.h"
#include "timer.h"
#include "VM_device.h"

typedef struct Class_lamp lamp_t;
struct Class_lamp
{
  /* base */
  DMEM *mem;
  void (*deinit)(lamp_t *lamp);

  /* attribute */
  float voltage;
  float current;
	float light_val;
  int isEnable;

  /* operation */
  void (*update)(lamp_t *lamp, int sysytime);
  void (*enable)(lamp_t *lamp);
  void (*disable)(lamp_t *lamp);

  /* object */
  vkey_t *key_0;
  vkey_t *key_1;
  vkey_t *key_2;
  light_t *light;
  inductor_t *inductor;
  panel_lamp_t *panel;
  mTimer_t *timer;
	device_t *zigbee;
	device_t *vol;
	device_t *light_sensor;
	device_t *blueTeeth;
};
lamp_t *Class_lamp_init(void);
#endif