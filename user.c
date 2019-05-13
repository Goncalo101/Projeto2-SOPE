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
#include "sope.h"

int main(int argc, char* argv[])
{
    char final[50];
    create_name_fifo(final, getpid());

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
    logRequest(STDOUT_FILENO, getpid(), &t);

    //creates fifo that will accomodate answer from server side (answer fifo)
    //TODO: add right name to fifo
    mkfifo(final, 0660);
    int fifo = open(SERVER_FIFO_PATH, O_WRONLY);

    printf("aaaaa\n");
    //writes to server(fifo) the order
    write_fifo_server(fifo, &t);
    //opens answer(fifo) to recive answer from server

    tlv_reply_t reply;
    // read_fifo_answer(final, &reply);
    printf("ccccccc\n");


    unlink(final);
    return 0;
}