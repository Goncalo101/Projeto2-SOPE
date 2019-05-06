#include "constants.h"
#include "types.h"
#include <fcntl.h>
#include "info.h"
#include <stdio.h>
#include <stdlib.h>


void processInformation(req_header_t *t)
{
    t->pid = getpid();

}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //handles information from arguments to a struct
    Info info = addInfo(argv);

    //creates name of fifo to create to read message from server
    char fifopath[USER_FIFO_PATH_LEN];
    int pid = getpid();
    strcat(fifopath, pid);

    //creates fifo
    mkfifo(fifopath, 0660);

    //opens fifo to send message into
    int fd = open(SERVER_FIFO_PATH, O_WRONLY);
    if(fd == -1)
    {
        printf("No fifo open \n");
        exit(1);
    }

    return 0;
}