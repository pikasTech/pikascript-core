#ifndef __VM_DHT11_H
#define __VM_DHT11_H
#include "VM_memory.h"
typedef struct VM_dht11
{
    DMEM *mem;
    unsigned char data[5];
    int tmp;
    int hum;

    void (*DeInit)(struct VM_dht11 *);
    int (*getTem)(struct VM_dht11 *);
    int (*getHum)(struct VM_dht11 *);
    void (*update)(struct VM_dht11 *);
} dht_t;

dht_t * VM_dht11_init(void);

#ifndef LINUX
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "tim.h"

#define GPIO_DHT11_PORT GPIOA
#define GPIO_DHT11_PIN GPIO_PIN_1
#define DHT11_IN HAL_GPIO_ReadPin(GPIO_DHT11_PORT, GPIO_DHT11_PIN)
#define DHT11_OUT_L HAL_GPIO_WritePin(GPIO_DHT11_PORT, GPIO_DHT11_PIN, GPIO_PIN_RESET)
#define DHT11_OUT_H HAL_GPIO_WritePin(GPIO_DHT11_PORT, GPIO_DHT11_PIN, GPIO_PIN_SET)
    uint16_t dht11_read_data(uint8_t *buffer);

#ifdef __cplusplus
}
#endif
#endif
#endif
