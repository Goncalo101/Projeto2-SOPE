#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "accounts.h"
#include "answerfifoaux.h"
#include "communication.h"
#include "constants.h"
#include "sope.h"
#include "types.h"

int main(int argc, char *argv[])
{

    if (argc != 3 || atoi(argv[1]) > MAX_BANK_OFFICES)
    {
        printf("Wrong Usage: server <front office nr (<= %d)> <admin password>\n",
               MAX_BANK_OFFICES);
        exit(1);
    }

    int shutdown = 0;

    // create admin account
    create_admin_account(argv[2]);

    // TODO: add balconies

    // create fifo to send information (server)
    mkfifo(SERVER_FIFO_PATH, 0660);

    // main loop
    while (!shutdown)
    {

        // reads from server(fifo) info send by user
        tlv_request_t request;
        read_fifo_server(SERVER_FIFO_PATH, &request);

        ret_code_t return_code = 0;
        rep_header_t header;
        tlv_reply_t t;

        printf("type: %d \n", request.type);

        switch (request.type) // TODO: catch return codes
        {
        case 0: // create account
        {
            return_code = create_account(
                request.value.create.password, request.value.create.balance,
                request.value.create.account_id, request.value.header.account_id);
            create_header_struct_a(getpid(), return_code, &header);
            t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
            break;
        }
        case 1: // balance check
        {
            rep_balance_t balance;
            int balance_nbr = 0;
            handleBalanceRequest(request.value.header.op_delay_ms,
                                 request.value.header.account_id, &balance_nbr);
            create_balance_struct_a(balance_nbr, &balance);
            t = join_structs_to_send_a(0, &header, &balance, NULL, NULL);
            break;
        }
        case 2: // transference
        {
            rep_transfer_t transfer;
            return_code = transfer_money(request.value.header.account_id,
                                         request.value.transfer.account_id,
                                         request.value.transfer.amount);
            create_header_struct_a(getpid(), return_code, &header);
            transfer.balance = accounts[request.value.header.account_id].balance;
            t = join_structs_to_send_a(0, &header, NULL, &transfer, NULL);
            break;
        }
        case 3: // shutdown
        {
            printf("aaaaaaa\n");
            int active;
            rep_shutdown_t shutdown_str;
            handle_shutdown(request.value.header.account_id, &shutdown, &active);
            printf("bbbbbbb\n");
            create_shutdown_struct_a(0, &shutdown_str); //TODO:add real numnber of active banks(when threads)
            printf("ccccccc\n");
            t = join_structs_to_send_a(0, &header, NULL, NULL, &shutdown_str);
            printf("ddddddd\n");
            break;
        }
        }
        
          printf("fffffff\n");
        // writes answer to user by answer (fifo)
        write_fifo_answer(USER_FIFO_PATH_PREFIX, &t);
    }

    printf("bshjfmg,h\n");

    return 0;
}
