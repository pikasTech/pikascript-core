#include "VM_dht11_test.h"
#include "VM_dht11.h"

int TEST_VM_dht11(int isShow)
{
    dht_t *dht = VM_dht11_init();
    int tem;
    int hum;
    dht->update(dht);

    tem = dht->getTem(dht);
    hum = dht->getHum(dht);
    dht->DeInit(dht);

    if (isShow)
    {
        printf("the resulat of dht11 is [tem: %d, hum: %d]\r\n", tem, hum);
    }
    if (33 != tem)
    {
        return 1;
    }
    if (22 != hum)
    {
        return 2;
    }
    return 0;
}