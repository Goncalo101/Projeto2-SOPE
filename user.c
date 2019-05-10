#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "communication.h"
#include "constants.h"
#include "serverfifoaux.h"
#include "types.h"
#include "userflag.h"

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    User_flag flag;
    if (addflag(argv, &flag) != 0) {
        printf("wrong arguments\n");
        exit(0);
    }

    //printTest(flag);
    tlv_request_t t = join_structs_to_send(flag);

    //creates fifo that will accomodate answer from server side (answer fifo)
    //TODO: add right name to fifo
    mkfifo(USER_FIFO_PATH_PREFIX, 0660);
    printf("aaaaa\n");
    //writes to server(fifo) the order
    write_fifo_server(SERVER_FIFO_PATH, &t);
    printf("bbbbbbb\n");
    sleep(2);
    //opens answer(fifo) to recive answer from server
    read_fifo_answer(USER_FIFO_PATH_PREFIX);
    printf("ccccccc\n");

    return 0;
}