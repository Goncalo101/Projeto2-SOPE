#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


#include "constants.h"
#include "types.h"
#include "accounts.h"
#include "communication.h"
#include "answerfifoaux.h"
#include "sope.h"


int main(int argc, char *argv[])
{

    if (argc != 3 || atoi(argv[1]) > MAX_BANK_OFFICES)
    {
        printf("Wrong Usage: server <front office nr (<= %d)> <admin password>\n", MAX_BANK_OFFICES);
        exit(1);
    }


    sleep(2);
    //create admin account
    create_account(argv[2], "test",0,0,0);

    //TODO: add balconies

    //create fifo to send information (server)
    mkfifo(SERVER_FIFO_PATH, 0660);

    //reads from server(fifo) info send by user
    read_fifo_server(SERVER_FIFO_PATH);

    tlv_reply_t t = join_structs_to_send_a(0); //TODO: add struct with info to pass

   // writes answer to user by answer (fifo)
    write_fifo_answer(USER_FIFO_PATH_PREFIX, &t);

    return 0;
}