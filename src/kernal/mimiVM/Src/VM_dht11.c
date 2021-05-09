#include "VM_dht11.h"
#include "VM_memory.h"
#ifndef LINUX
#include "tim.h"
/**
* @brief  Configure the pin as input
* @param  None
* @retval None
*/
static void dht11_gpio_input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_DHT11_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIO_DHT11_PORT, &GPIO_InitStruct);
}

/**
* @brief  Configure the pin as output
* @param  None
* @retval None
*/
static void dht11_gpio_output(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_DHT11_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIO_DHT11_PORT, &GPIO_InitStruct);
}

/**
* @brief  Reset the pin of DHT11 data pin
* @param  None
* @retval None
*/
static void dht11_reset(void)
{
  dht11_gpio_output();
  DHT11_OUT_L;
  delay_us(18000);
  DHT11_OUT_H;
  delay_us(30);
  dht11_gpio_input();
}

/**
* @brief  Read the state of DHT11 data pin
* @param  None
* @retval the pin state
*/
static uint16_t dht11_scan(void)
{
  return DHT11_IN;
}

/**
* @brief  Read a bit form DHT11 sensor
* @param  buffer: pointer to data buffer
* @retval bit value
*/
static uint16_t dht11_read_bit(void)
{
  int timeOut = 500;
  while ((DHT11_IN == GPIO_PIN_RESET) && (--timeOut > 0))
    ;
  delay_us(40);
  if (DHT11_IN == GPIO_PIN_SET)
  {
    while ((DHT11_IN == GPIO_PIN_SET) && (--timeOut > 0))
      ;

    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief  Read a byte form DHT11 sensor
* @param  None
* @retval byte value
*/
static uint16_t dht11_read_byte(void)
{
  uint16_t i;
  uint16_t data = 0;

  for (i = 0; i < 8; i++)
  {
    data <<= 1;
    data |= dht11_read_bit();
  }

  return data;
}

/**
* @brief  Read all data form DHT11 sensor
* @param  buffer: pointer to data buffer
* @retval 1: Success
0: Fail
*/
uint16_t dht11_read_data(uint8_t *buffer)
{
  uint16_t i = 0;
  int timeOut = 2000;
  dht11_reset();
  if (dht11_scan() == GPIO_PIN_RESET)
  {
    while ((dht11_scan() == GPIO_PIN_RESET) && (--timeOut > 0))
      ;
    while ((dht11_scan() == GPIO_PIN_SET) && (--timeOut > 0))
      ;
    for (i = 0; i < 5; i++)
    {
      buffer[i] = dht11_read_byte();
    }

    while ((dht11_scan() == GPIO_PIN_RESET) && (--timeOut > 0))
      ;

    uint8_t checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
    if (checksum == buffer[4])
    {
      return 1;
    }
  }

  return 0;
}
#endif
#ifdef LINUX
int dht11_read_data(unsigned char *buffer)
{
  buffer[0] = 22;
  buffer[1] = 22;
  buffer[2] = 33;
  buffer[3] = 33;
  buffer[4] = buffer[0] + buffer[1] + buffer[2] + buffer[3];
  return 1;
}
#endif

void VM_dht11_DeInit(dht_t *dht)
{
  DynMemPut(dht->mem);
}

int VM_dht11_getTem(dht_t *dht)
{
  return dht->tmp;
}

int VM_dht11_getHum(dht_t *dht)
{
  return dht->hum;
}

void VM_dht11_update(dht_t *dht)
{
  if (dht11_read_data(dht->data))
  {
    dht->tmp = dht->data[2];
    dht->hum = dht->data[0];
  }
}

dht_t *VM_dht11_init(void)
{
  DMEM *mem = DynMemGet(sizeof(dht_t));
  dht_t *dht = (dht_t *)(mem->addr);
  dht->mem = mem;
  dht->data[0] = 0;
  dht->tmp = 0;
  dht->hum = 0;

  dht->DeInit = VM_dht11_DeInit;
  dht->getTem = VM_dht11_getTem;
  dht->getHum = VM_dht11_getHum;
  dht->update = VM_dht11_update;

  return dht;
}
