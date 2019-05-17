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

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //--OPEN LOG FILE ---------------------
    userlog = open(USER_LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    //-------------------------------------

    //--PROCESS ARGUMENTS--------------
    User_flag flag;
    if (addflag(argv, &flag) != 0) {
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
    tlv_reply_t reply;

    logRequest(userlog, getpid(), &t);
    if (write_fifo_server(&t) == RC_SRV_DOWN) {
        reply.value.header.ret_code = RC_SRV_DOWN;
        logReply(userlog, getpid(), &reply);
    } else {
        //--READ REPLY FROM SERVER TO USER---------
        read_fifo_answer(final, &reply); //TODO:wait for 30s not forget RC_TIMEOUT
        logReply(userlog, getpid(), &reply);
        //-------------------------------------
    }

    unlink(final);
    return 0;
}
