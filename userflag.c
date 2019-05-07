#include "userflag.h"
#include <stdio.h>
#include <stdlib.h>

User_flag addflag(char *argv[])
{
    //TODO: Add verification to opnumber value
    User_flag flag;
    flag.id = atoi(argv[1]);
    strncpy(flag.password, argv[2], sizeof(flag.password));
    flag.delay = atoi(argv[3]);
    flag.opnumber = atoi(argv[4]);
    strncpy(flag.arguments, argv[5], sizeof(flag.arguments));

    return flag;
}

void printTest(User_flag flag)
{
    printf("id %d \n", flag.id);
    printf("pass %s \n", flag.password);
    printf("delay %d \n", flag.delay);
    printf("opnumber %d \n", flag.opnumber);
    printf("arguments %s \n", flag.arguments);
}