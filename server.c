#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "accounts.h"
#include "answerfifoaux.h"
#include "communication.h"
#include "linked_list.h"
#include "sope.h"
#include "types.h"
#include "argcheck.h"

static node_t *request_queue;
uint32_t shutdown = 0;
static int serverlog;

sem_t empty, full;

int get_sem_value(sem_t *t)
{
    int value;
    if (sem_getvalue(t, &value) != -1)
        return value;
    else
        return -1;
}
tlv_request_t get_request()
{
    tlv_request_t request = request_queue->val;
    pop(&request_queue);
    return request;
}

void *operations(void *nr)
{
    int number_office = *(int *)nr;
    printf("numb offices %d", number_office);

    ret_code_t return_code;
    rep_header_t header;
    tlv_reply_t t;
    tlv_request_t request;

    while (!shutdown)
    {
        logSyncMechSem(serverlog, number_office, SYNC_OP_SEM_WAIT, SYNC_ROLE_CONSUMER, 0, get_sem_value(&full));
        sem_wait(&full);
        request = get_request();
        logRequest(serverlog, number_office, &request);

        return_code = authenticate_user(request.value.header.account_id, request.value.header.op_delay_ms, request.value.header.password, serverlog);
        if (return_code != 0)
        {
            create_header_struct_a(request.value.create.account_id, return_code, &header);
            t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
        }
        else
        {
            switch (request.type) // TODO: catch return codes
            {
            case 0: // create account
            {
                if (return_code == 0)
                {
                    return_code = create_account(
                        request.value.create.password, request.value.create.balance,
                        request.value.create.account_id, request.value.header.account_id, request.value.header.op_delay_ms, serverlog, number_office);
                }
                create_header_struct_a(request.value.create.account_id, return_code, &header);
                t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
                break;
            }
            case 1: // balance check
            {
                rep_balance_t balance;
                uint32_t balance_nbr = 0;
                handle_balance_request(request.value.header.op_delay_ms,
                                       request.value.header.account_id, &balance_nbr, serverlog);
                create_balance_struct_a(balance_nbr, &balance);
                t = join_structs_to_send_a(1, &header, &balance, NULL, NULL);
                break;
            }
            case 2: // transference
            {
                rep_transfer_t transfer;
                return_code = transfer_money(request.value.header.account_id,
                                             request.value.transfer.account_id,
                                             request.value.transfer.amount, request.value.header.op_delay_ms, serverlog);
                create_header_struct_a(request.value.header.account_id, return_code, &header);
                transfer.balance = accounts[request.value.header.account_id].balance;
                t = join_structs_to_send_a(2, &header, NULL, &transfer, NULL);
                break;
            }
            case 3: // shutdown
            {
                uint32_t active;
                rep_shutdown_t shutdown_str;
                handle_shutdown(request.value.header.account_id, &shutdown, &active, request.value.header.op_delay_ms, serverlog);
                create_shutdown_struct_a(0, &shutdown_str); //TODO:add real numnber of active banks(when threads)
                t = join_structs_to_send_a(3, &header, NULL, NULL, &shutdown_str);
                break;
            }
            }
        }

        // writes answer to user by answer (fifo)
        char final[50];
        create_name_fifo(final, request.value.header.pid);
        int fifo_answer_write = open(final, O_WRONLY);
        if (fifo_answer_write == -1)
            return RC_USR_DOWN;
        logReply(serverlog, number_office, &t);
        write_fifo_answer(fifo_answer_write, &t);
        sem_post(&empty);
        logSyncMechSem(serverlog, number_office, SYNC_OP_SEM_POST, SYNC_ROLE_CONSUMER, 0, get_sem_value(&empty));
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3 || check_server_arguments(argv[1], argv[2]))
    {
        printf("Wrong Usage: server <front office nr (<= %d )> <admin password> \n", MAX_BANK_OFFICES);
        exit(1);
    }

    int nbr_balconies = atoi(argv[1]);

    create_admin_account(argv[2], serverlog);

    mkfifo(SERVER_FIFO_PATH, 0660);

    serverlog = open(SERVER_LOGFILE, O_WRONLY | O_CREAT, 0644);

    int fifo_server_read = open(SERVER_FIFO_PATH, O_RDONLY);
    int fifo_server_write = open(SERVER_FIFO_PATH, O_WRONLY);
    if (fifo_server_read == -1 || fifo_server_write == -1)
        return RC_SRV_DOWN;

    sem_init(&empty, 0, 1);
    logSyncMechSem(serverlog, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&empty));
    sem_init(&full, 0, 0);
    logSyncMechSem(serverlog, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&full));

    pthread_t tidf[nbr_balconies];
    int ids[nbr_balconies];

    for (int k = 0; k < nbr_balconies; k++)
    {
        ids[k] = k + 1;
        pthread_create(&tidf[k], NULL, operations, &ids[k]);
        logBankOfficeOpen(serverlog, 0, 0);
    }

    tlv_request_t request;
    while (!shutdown)
    {
        logSyncMechSem(serverlog, 0, SYNC_OP_SEM_WAIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&empty)); //TODO: add in NULL and check empty
        sem_wait(&empty);
        read_fifo_server(fifo_server_read, &request);
        logRequest(serverlog, 0, &request);

        if (request_queue == NULL)
        {
            request_queue = malloc(sizeof(node_t));
            request_queue->val = request;
            request_queue->next = NULL;
        }
        else
        {
            push(request_queue, request);
        }

        sem_post(&full);
        logSyncMechSem(serverlog, 0, SYNC_OP_SEM_POST, SYNC_ROLE_PRODUCER, 0,get_sem_value(&full));
    }

    for (int k = 0; k < nbr_balconies; k++)
    {
        pthread_join(tidf[k], NULL);
    }

    close(fifo_server_read);
    close(fifo_server_write);
    unlink(SERVER_FIFO_PATH);
    return 0;
}
