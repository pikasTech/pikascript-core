#include "add.h"

#include <stdio.h>


int main(int argc, char **argv)
{
    int a = 10;
    int b = 8;

    printf("%d + %d = %d\n", a, b, add(a, b));

    return 0;
}