#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "accounts.h"
#include "answerfifoaux.h"
#include "argcheck.h"
#include "communication.h"
#include "linked_list.h"
#include "sope.h"
#include "thread_args.h"
#include "types.h"

static node_t* request_queue;
uint32_t shutdown = 0;
static int serverlog;
int nbr_balconies = 0;
int interrupted = 0;

sem_t empty, full;

void sigusr_handler(int sig)
{
    if (sig == SIGUSR1)
        interrupted = 1;
}

int get_sem_value(sem_t* t)
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

//Balconies' operations
void* operations(void* nr)
{

    thread_args_t args = *(thread_args_t*)nr;
    int number_office = args.nr_office;
    pthread_t main_thread = args.main_thread;

    ret_code_t return_code;
    rep_header_t header;
    tlv_reply_t t;
    tlv_request_t request;

    while (!(shutdown && list_size_empty(request_queue))) {
        logSyncMechSem(serverlog, number_office, SYNC_OP_SEM_WAIT, SYNC_ROLE_CONSUMER, 0, get_sem_value(&full));
        sem_wait(&full);

        if (list_size_empty(request_queue)) {
            break;
        }

        change_active(serverlog, number_office, ADD_ACTIVE_THREAD);

        //Get request from queue
        request = get_request();
        logRequest(serverlog, number_office, &request);

        return_code = authenticate_user(request.value.header.account_id, request.value.header.op_delay_ms, request.value.header.password, serverlog, number_office);
        if (return_code != 0) {
            create_header_struct_a(request.value.create.account_id, return_code, &header);
            t = join_structs_to_send_a(request.type, &header, NULL, NULL, NULL);
        } else {
            switch (request.type) {
            case 0: //create account
            {
                if (return_code == 0) {
                    return_code = create_account(
                        request.value.create.password, request.value.create.balance,
                        request.value.create.account_id, request.value.header.account_id, request.value.header.op_delay_ms, serverlog, number_office);
                }

                create_header_struct_a(request.value.create.account_id, return_code, &header);
                t = join_structs_to_send_a(0, &header, NULL, NULL, NULL);
                break;
            }
            case 1: //balance check
            {
                rep_balance_t balance;
                uint32_t balance_nbr = 0;

                handle_balance_request(request.value.header.op_delay_ms,
                    request.value.header.account_id, &balance_nbr, serverlog, number_office);
                create_balance_struct_a(balance_nbr, &balance);
                t = join_structs_to_send_a(1, &header, &balance, NULL, NULL);
                break;
            }
            case 2: //transference
            {
                rep_transfer_t transfer;
                uint32_t balance = 0;
                return_code = transfer_money(request.value.header.account_id,
                    request.value.transfer.account_id,
                    request.value.transfer.amount, request.value.header.op_delay_ms, serverlog, number_office, &balance);
                create_header_struct_a(request.value.header.account_id, return_code, &header);
                create_transfer_struct_a(balance, &transfer);
                t = join_structs_to_send_a(2, &header, NULL, &transfer, NULL);

                break;
            }
            case 3: //shutdown
            {
                uint32_t active;
                rep_shutdown_t shutdown_str;

                return_code = handle_shutdown(request.value.header.account_id, &shutdown, &active, request.value.header.op_delay_ms, serverlog, number_office);
                create_header_struct_a(request.value.header.account_id, return_code, &header);
                create_shutdown_struct_a(active, &shutdown_str);
                t = join_structs_to_send_a(3, &header, NULL, NULL, &shutdown_str);

                if (request.value.header.account_id == 0) {
                    for (int i = 0; i < nbr_balconies; i++) {
                        sem_post(&full);
                        logSyncMechSem(serverlog, number_office, SYNC_OP_SEM_POST, SYNC_ROLE_CONSUMER, 0, get_sem_value(&full));
                    }
                    pthread_kill(main_thread, SIGUSR1);
                }

                break;
            }
            default:
                break;
            }
        }
        sem_post(&empty);
        logSyncMechSem(serverlog, number_office, SYNC_OP_SEM_POST, SYNC_ROLE_CONSUMER, request.value.header.pid, get_sem_value(&empty));

        //Write reply to /tmp/secure_XXXXX
        char final[50];
        create_name_fifo(final, request.value.header.pid);
        write_fifo_answer(final, &t);
        logReply(serverlog, number_office, &t);
        change_active(serverlog, number_office, REMOVE_ACTIVE_THREAD);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 3 || check_server_arguments(argv[1], argv[2])) {
        printf("Wrong Usage: server <front office nr (<= %d )> <admin password> \n", MAX_BANK_OFFICES);
        exit(1);
    }

    nbr_balconies = atoi(argv[1]);

    //Open log file
    serverlog = open(SERVER_LOGFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    create_admin_account(argv[2], serverlog);

    //Create fifo /tmp/secure_srv
    mkfifo(SERVER_FIFO_PATH, 0660);

    struct sigaction action;
    action.sa_handler = sigusr_handler;
    sigaction(SIGUSR1, &action, NULL);

    //Initialize semaphores
    logSyncMechSem(serverlog, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&empty));
    sem_init(&empty, 0, 1);
    logSyncMechSem(serverlog, 0, SYNC_OP_SEM_INIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&full));
    sem_init(&full, 0, 0);

    pthread_t tidf[nbr_balconies];
    thread_args_t ids[nbr_balconies];

    //Initialize balconies
    for (int k = 0; k < nbr_balconies; k++) {
        ids[k].nr_office = k + 1;
        ids[k].main_thread = pthread_self();
        pthread_create(&tidf[k], NULL, operations, &ids[k]);
        logBankOfficeOpen(serverlog, 0, tidf[k]);
    }

    tlv_request_t request;
    int read_srv = 1;

    //Process requests from users
    while (!(shutdown && (read_srv == 0))) {
        logSyncMechSem(serverlog, 0, SYNC_OP_SEM_WAIT, SYNC_ROLE_PRODUCER, 0, get_sem_value(&empty)); //TODO: add in NULL and check empty
        sem_wait(&empty);
        if (interrupted)
            break;
        read_srv = read_fifo_server(&request);

        if (!interrupted) {
            logRequest(serverlog, 0, &request);

            if (request_queue == NULL) {
                request_queue = malloc(sizeof(node_t));
                request_queue->val = request;
                request_queue->next = NULL;
            } else {
                push(request_queue, request);
            }
        }

        sem_post(&full);
        logSyncMechSem(serverlog, 0, SYNC_OP_SEM_POST, SYNC_ROLE_PRODUCER, request.value.header.pid, get_sem_value(&full));
        if (interrupted)
            break;
    }

    for (int k = 0; k < nbr_balconies; k++) {
        pthread_join(tidf[k], NULL);
        logBankOfficeClose(serverlog, 0, tidf[k]);
    }

    unlink(SERVER_FIFO_PATH);
    return 0;
}
