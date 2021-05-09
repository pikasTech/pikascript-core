#include "VM_socket.h"
VM_socket_t *socket_1;
VM_socket_t *socket_2;

int TEST_VM_socket(int isShow)
{
    socket_1 = VM_socket_init();
    socket_2 = VM_socket_init();
    socket_1->isShowInfo = isShow;
    socket_2->isShowInfo = isShow;

    socket_1->open(socket_1);
    socket_2->open(socket_2);

    socket_1->close(socket_1);
    socket_2->close(socket_2);

    socket_1->DeInit(socket_1);
    socket_1->DeInit(socket_2);
    return 0;
}
