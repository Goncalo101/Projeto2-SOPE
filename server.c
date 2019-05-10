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
    create_admin_account(argv[2]);
    printf("dbhcnjxmkl\n");

    //TODO: add balconies

    //create fifo to send information (server)
    mkfifo(SERVER_FIFO_PATH, 0660);
        printf("hhhhhhhh\n");

    //reads from server(fifo) info send by user
    tlv_request_t request;
    sleep(1);
    read_fifo_server(SERVER_FIFO_PATH, &request);
            printf("uiuiuiui\n");

    ret_code_t return_code = 0;
    rep_header_t header;
    tlv_reply_t t;

    switch (request.type) //TODO: catch return codes
    {
    case 0: //create account
    {
        printf("pppppp\n");
        printf("pass %s \n", request.value.create.password);
        printf("balance %d \n", request.value.create.balance);
        printf("id %d \n", request.value.create.account_id);

        return_code = create_account(request.value.create.password, request.value.create.balance, request.value.create.account_id, request.value.header.account_id);
                printf("gggggg\n");
        create_header_struct_a(getpid(), return_code, &header);
                printf("vvvvvvv\n");
        t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
                printf("jjjjjjjj\n");
        break;
    }
    case 1: //balance check
    {
        rep_balance_t balance;
        int balance_nbr = 0;
        handleBalanceRequest(request.value.header.op_delay_ms, request.value.header.account_id,&balance_nbr);
        create_balance_struct_a(balance_nbr,&balance);
        t = join_structs_to_send_a(0, &header,&balance, NULL, NULL);
        break;
    }
    case 2: //transference
    {
        rep_transfer_t transfer;
        return_code = transfer_money(request.value.header.account_id, request.value.transfer.account_id, request.value.transfer.amount);
        create_header_struct_a(getpid(), return_code, &header);
        transfer.balance = accounts[request.value.header.account_id].balance;
        t = join_structs_to_send_a(0, &header,NULL, &transfer, NULL);
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
    printf("bshjfmg,h\n");

    return 0;
}
