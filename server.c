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
    create_account(argv[2], 0, 0, 0);

    //TODO: add balconies

    //create fifo to send information (server)
    mkfifo(SERVER_FIFO_PATH, 0660);

    //reads from server(fifo) info send by user
    tlv_request_t request;
    read_fifo_server(SERVER_FIFO_PATH, &request);

    int return_code = 0;
    rep_header_t header;
    tlv_reply_t t;

    switch (request.type) //TODO: catch return codes
    {
    case 0: //create account
    {
        return_code = create_account(request.value.create.password, request.value.create.balance, request.value.create.account_id, request.value.header.account_id);
        create_header_struct_a(getpid(), return_code, &header);
        t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
        break;
    }
    case 1: //balance check
    {
        // rep_balance_t balance;
        // //add operation and fill elements of balance with information based on operation
        // t = join_structs_to_send_a(0, &header,&balance, NULL, NULL);
        break;
    }
    case 2: //transference
    {
        // rep_transfer_t transfer;
        //   t = join_structs_to_send_a(0, &header,NULL, &transfer, NULL);
        break;
    }
    case 3: //shutdown
    {
        // rep_shutdown_t shutdown;
        //   t = join_structs_to_send_a(0, &header,NULL,NULL, &shutdown);
        break;
    }
    }


    // writes answer to user by answer (fifo)
    write_fifo_answer(USER_FIFO_PATH_PREFIX, &t);

    return 0;
}
