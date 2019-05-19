#include <fcntl.h>
#include <signal.h>
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

void sigalrm_handler(int sig)
{
    if (sig == SIGALRM)
        return;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //Open log file
    userlog = open(USER_LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0644);

    //Initialize alarm signal for timeout managment
    struct sigaction action;
    action.sa_handler = sigalrm_handler;
    sigaction(SIGALRM, &action, NULL);

    //Process arguments
    User_flag flag;
    if (addflag(argv, &flag) != 0)
    {
        printf("Wrong arguments\n");
        exit(0);
    }

    tlv_request_t t = join_structs_to_send(flag);

    //Create fifo /tmp/secure_XXXXX
    char final[50];
    create_name_fifo(final, getpid());
    mkfifo(final, 0660);

    //Write request to /tmp/secure_srv
    tlv_reply_t reply;
    logRequest(userlog, getpid(), &t);
    int fifo_server_write = write_fifo_server(&t);
    if (fifo_server_write == -1)
    {
        reply.value.header.ret_code = RC_SRV_DOWN;
        logReply(userlog, getpid(), &reply);
    }
    else
    {
        //Read reply from /tmp/secure_XXXXX
        read_fifo_answer(final, &reply);
        logReply(userlog, getpid(), &reply);
    }

    close(fifo_server_write);
    unlink(final);
    return 0;
}
