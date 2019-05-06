#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include "constants.h"
#include "types.h"
#include "accounts.h"
#include "communication.h"


int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //create admin account
    create_account(argv[2], "test", 0);

    //TODO: add balconies

    //create fifo to send information (server)
    mkfifo(SERVER_FIFO_PATH, 0660);

    //reads from server(fifo) info send by user
    read_fifo(SERVER_FIFO_PATH);

    //writes answer to user by answer (fifo)
    write_fifo(USER_FIFO_PATH_PREFIX, "goodbye\n");

    return 0;
}