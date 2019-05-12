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
    // int n = 3;
    // main loop
    while (!shutdown)
    {

        // reads from server(fifo) info send by user
        tlv_request_t request;
        // request.type = 0;
        // request.value.header.account_id = 0;
        // strncpy(request.value.header.password, "admin", sizeof(request.value.create.password));
        // request.value.header.pid = 11258;
        // request.value.create.account_id = 1;
        // request.value.create.balance = 100;
        // strncpy(request.value.create.password, "loureiroruivo", sizeof(request.value.create.password));

        // tlv_request_t request2;
        // request2.type = 0;
        // request2.value.header.account_id = 0;
        // strncpy(request2.value.header.password, "admin", sizeof(request.value.create.password));
        // request2.value.header.pid = 11258;
        // request2.value.create.account_id = 2;
        // request2.value.create.balance = 200;
        // strncpy(request2.value.create.password, "loureiroruivo", sizeof(request.value.create.password));

        // tlv_request_t request3;
        // request3.type = 2;
        // request3.value.header.account_id = 1;
        // strncpy(request3.value.header.password, "loureiroruivo", sizeof(request.value.create.password));
        // request3.value.header.pid = 11258;
        // request3.value.transfer.account_id = 2;
        // request3.value.transfer.amount= 50;



        // int p = 0;
        // if (n == 3)
        //     p = request.type;
        // else if( n == 2)
        // {
        //     p = 4;
        // }
        // else p = request3.type;

        read_fifo_server(SERVER_FIFO_PATH, &request);

        ret_code_t return_code = 0;
        rep_header_t header;
        tlv_reply_t t;
        tlv_reply_t t2;
          tlv_reply_t t3;

        switch (request.type) // TODO: catch return codes
        {
        case 0: // create account
        {
            return_code = create_account(
                request.value.create.password, request.value.create.balance,
                request.value.create.account_id, request.value.header.account_id);
            create_header_struct_a(request.value.create.account_id, return_code, &header);
            t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
            break;
        }
        case 1: // balance check
        {
            rep_balance_t balance;
            int balance_nbr = 0;
            handle_balance_request(request.value.header.op_delay_ms,
                                   request.value.header.account_id, &balance_nbr);
            create_balance_struct_a(balance_nbr, &balance);
            t = join_structs_to_send_a(1, &header, &balance, NULL, NULL);
            break;
        }
        case 2: // transference
        {
            rep_transfer_t transfer;
            return_code = transfer_money(request.value.header.account_id,
                                         request.value.transfer.account_id,
                                         request.value.transfer.amount);
            create_header_struct_a(request.value.header.account_id, return_code, &header);
            transfer.balance = accounts[request.value.header.account_id].balance;
            t = join_structs_to_send_a(2, &header, NULL, &transfer, NULL);
            break;
        }
        case 3: // shutdown
        {
            int active;
            rep_shutdown_t shutdown_str;
            handle_shutdown(request.value.header.account_id, &shutdown, &active);
            create_shutdown_struct_a(0, &shutdown_str); //TODO:add real numnber of active banks(when threads)
            t = join_structs_to_send_a(3, &header, NULL, NULL, &shutdown_str);
            break;
        }
        }

        // printf("----------------------------- \n");
        // printf("type %d \n", t.type);
        // printf("ret code %d \n", t.value.header.ret_code);
        // printf("new account id %d \n", t.value.header.account_id);
        // printf("----------------------------- \n");

        // printf("----------------------------- \n");
        // printf("type t2 %d \n", t2.type);
        // printf("ret code t2 %d \n", t2.value.header.ret_code);
        // printf("new account id t2 %d \n", t2.value.header.account_id);
        // printf("----------------------------- \n");

        // printf("----------------------------- \n");
        // printf("balance t3: %d \n", t3.value.transfer.balance);
        // printf("type t3: %d \n", t3.type);
        // printf("ret code t3: %d \n", t3.value.header.ret_code);
        // printf("id t3  %d \n", t3.value.header.account_id);
        // printf("---------fimmmm----- \n");
        // n--;


       // writes answer to user by answer (fifo)
        char final[50];
        create_name_fifo(final, request.value.header.pid);
        write_fifo_answer(final, &t);
    }

    //unlink(SERVER_FIFO_PATH);
    return 0;
}
