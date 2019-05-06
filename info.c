#include "info.h"
#include <stdio.h>
#include <stdlib.h>

Info addInfo(char *argv[])
{
    //TODO: Add verification to opnumber value
    Info info;
    info.id = atoi(argv[1]);
    strncpy(info.password, argv[2], sizeof(info.password));
    info.delay = atoi(argv[3]);
    info.opnumber = atoi(argv[4]);
    strncpy(info.arguments, argv[5], sizeof(info.arguments));

    return info;
}

void printTest(Info info)
{
    printf("id %d \n", info.id);
    printf("pass %s \n", info.password);
    printf("delay %d \n", info.delay);
    printf("opnumber %d \n", info.opnumber);
    printf("arguments %s \n", info.arguments);
}