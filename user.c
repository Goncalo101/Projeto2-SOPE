#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


#include "communication.h"
#include "serverfifoaux.h"
#include "userflag.h"
#include "constants.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    User_flag flag = addflag(argv);    
    
sleep(2);
    //creates fifo that will accomodate answer from server side (answer fifo)
    //TODO: add right name to fifo
    mkfifo(USER_FIFO_PATH_PREFIX, 0660); 

    //writes to server(fifo) the order
    write_fifo(SERVER_FIFO_PATH, "hi bitch \n");

    //opens answer(fifo) to recive answer from server
    read_fifo(USER_FIFO_PATH_PREFIX);

    return 0;
}