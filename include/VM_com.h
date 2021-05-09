#ifndef __VM_COM_H
#define __VM_COM_H
#include "VM_memory.h"
typedef struct VM_com
{
    DMEM *mem;
    char RX_Char[1];
    
    char RX_IT;
    int RX_buff_i;
    char RX_buff[256];
    char RX_SingleLine[256];
    char RX_IT_SingleLine;

    char TX_buff[256];
    char TX_vIT;
    char name[16];
    char CMDOut[256];

    void (*DeInit)(struct VM_com *);

    // push the pdata to buff, and the data would be send asynchronous autoly
    void (*send)(struct VM_com *, char *);
    char *(*sendWithName)(struct VM_com *, char *, char *);
    // the call back, called by hardware IT
    void (*IT_CallBack)(struct VM_com *);
    // the virtual call back, called by task or tim IT
    void (*vIT_CallBack)(struct VM_com *);

    // init the hardware
    void (*DriverInit)(struct VM_com *);
    // synchronous send to hardware
    void (*DriverSend)(struct VM_com *);

    //whither to show resault in test
    int isShow;
    //used to check the ouSize of TX_buff
    int outSize;

} VM_com_t;

VM_com_t *VM_com_init_v2(void);
// the rewrite of DriverSend 
void VMdriver_comTransmitHuart2(VM_com_t *pcom);


#endif