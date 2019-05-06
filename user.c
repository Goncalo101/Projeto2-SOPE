#include "constants.h"
#include "types.h"
#include <fcntl.h>
#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "communication.h"

/*void processInformation(req_header_t *t)
{
    t->pid = getpid();

}*/

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //handles information from arguments to a struct
    //Info info = addInfo(argv);

    //creates name of fifo to create to read message from server
    /*char fifopid[USER_FIFO_PATH_LEN];
    //char *cpid;
    //sprintf(cpid, "%d", getpid());
    strcat(fifopid, USER_FIFO_PATH_PREFIX);
    strcat(fifopid, "20000");*/


    //creates fifo that will accomodate answer from server side (answer fifo)
    mkfifo(USER_FIFO_PATH_PREFIX, 0660);

    //writes to server(fifo) the order
    write_fifo(SERVER_FIFO_PATH, "hi bitch \n");

    //opens answer(fifo) to recive answer from server
    read_fifo(USER_FIFO_PATH_PREFIX);

    return 0;
}