#ifndef __VM_callBack_H
#define __VM_callBack_H
typedef struct VM_callBack callBack_t;
struct VM_callBack
{
    void (*fun)(void *input);
    void *input;
};

#endif