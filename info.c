#include "info.h"

Info *addInfo(char *argv[])
{
    //TODO: Add verification to opnumber value
    Info info;
    info.id = argv[0];
    strncpy(info.password, argv[1], sizeof(info.password));
    info.delay = argv[2];
    info.opnumber = argv[3];
    strncpy(info.arguments, argv[4], sizeof(info.arguments));

    return &info;
}

void printTest(Info *info)
{
    printf("id %d \n", info->id);
    printf("pass %s \n", info->password);
    printf("delay %d \n", info->delay);
    printf("opnumber %d \n", info->opnumber);
    printf("arguments %s \n", info->arguments);
}