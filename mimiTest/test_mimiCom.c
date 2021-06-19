#include "test_mimiCom.h"
#include "mimiCom.h"

int TEST_mimiCom(int isShow)
{
    mimiCom_t *com = New_mimiCom(NULL);

    com->getChar(com, 'a');
    com->getChar(com, 'a');
    com->getChar(com, 'a');
    com->getChar(com, '\r');
    com->getChar(com, '\n');

    char *RxSingleLine = NULL;
    com->args->getStr(com->args, "RxSingleLine", &RxSingleLine);
    if (isShow)
    {
        printf("Single Line is: %s\r\n", RxSingleLine);
    }

    com->dinit(com);
    return 0;
}