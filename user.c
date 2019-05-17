#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "communication.h"
#include "serverfifoaux.h"
#include "sope.h"
#include "types.h"
#include "userflag.h"

static int userlog;

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //--OPEN LOG FILE ---------------------
    userlog = open(USER_LOGFILE, O_WRONLY | O_CREAT |O_APPEND , 0644);
    //-------------------------------------

    //--PROCESS ARGUMENTS--------------
    User_flag flag;
    if (addflag(argv, &flag) != 0)
    {
        printf("wrong arguments\n");
        exit(0);
    }

    tlv_request_t t = join_structs_to_send(flag);
    //-------------------------------------

    //--CREATE ANSWER FIFO----------------
    char final[50];
    create_name_fifo(final, getpid());
    mkfifo(final, 0660);
    //-------------------------------------

    //--WRITE REQUEST FROM USER TO SERVER -----
    int fifo_server_write = open(SERVER_FIFO_PATH, O_WRONLY);
    if (fifo_server_write == -1)
        return RC_SRV_DOWN;
    logRequest(userlog,getpid(),&t);
    write_fifo_server(fifo_server_write, &t);
    close(fifo_server_write);
    //-------------------------------------

    //--READ REPLY FROM SERVER TO USER---------
    tlv_reply_t reply;
    int fifo_answer_read = open(final, O_RDONLY);
    int fifo_answer_write = open(final, O_WRONLY);
    if (fifo_answer_read == -1 || fifo_answer_write == -1)
        return RC_USR_DOWN;

    read_fifo_answer(fifo_answer_read, &reply); //TODO:wait for 30s
    logReply(userlog, getpid(), &reply);
    //-------------------------------------

    close(fifo_answer_read);
    close(fifo_answer_write);
    unlink(final);
    return 0;
}
