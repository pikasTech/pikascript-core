#ifndef _market_case1__H
#define _market_case1__H
#include "VM_delay.h"
#include "VM_iot.h"
#include "VM_key.h"
#include "dataMemory.h"
#include "VM_socket.h"
#include "goods.h"
#include "panel_market.h"
typedef struct Class_market_case1 market_case1_t;
struct Class_market_case1
{
  DMEM *mem;
  void (*dinit)(market_case1_t *market_case1);

  panel_market_t *panel;

  goods_t *goods_main;

  goods_t *goods1;
  goods_t *goods2;
  goods_t *goods3;
  goods_t *goods4;
  goods_t *goods5;

  vkey_t *key_0;
  vkey_t *key_1;
  vkey_t *key_2;

  VM_socket_t *socket_0;
  VM_socket_t *socket_1;

  struct VM_delay *delay;

  iot_t *iot;

  int totle;

  int sales;
  int odders;

  void (*update)(market_case1_t *market_case1, int systime_ms);
};
market_case1_t *Class_market_case1_init(void);
#endif