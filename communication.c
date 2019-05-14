#include "communication.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void create_name_fifo(char* final, pid_t pid)
{
    char c_pid[5];
    strcpy(final, USER_FIFO_PATH_PREFIX);
    sprintf(c_pid, "%d", pid);
    strcat(final, c_pid);
}

void read_fifo_answer(char* path, tlv_reply_t* t)
{
    int fifo = open(path, O_RDONLY);
    while (fifo == -1) {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    printf("------------------------------------------- \n");
    printf("id account %u \n", t->value.header.account_id);
    printf("balance transfer account %u \n", t->value.transfer.balance);
    printf("balance %u \n", t->value.balance.balance);
    printf("------------------------------------------- \n");

    read(fifo, t, sizeof(*t));
    close(fifo);
}

void read_fifo_server(int fifo, tlv_request_t* t)
{
    printf("fd read: %d\n", fifo);

    int r = 0;

    while (r == 0)
        r = read(fifo, t, sizeof(*t));

    printf("read return: %d \n", r);

    printf("------------------------------------------- \n");
    printf("id create account %u \n", t->value.create.account_id);
    printf("balance create account %u \n", t->value.create.balance);
    printf("pass create account %s \n", t->value.create.password);
    printf("type %d \n", (int)t->type);
    printf("lenght %d \n", (int)t->length);
    printf("------------------------------------------- \n");
}

void write_fifo_server(int fifo, tlv_request_t* to_write)
{
    printf("fd write: %d\n", fifo);

    int w = write(fifo, to_write, sizeof(*to_write));
    printf("write return: %d \n", w);
}

//TODO: update to right struct
void write_fifo_answer(int fifo, tlv_reply_t* to_write)
{
    write(fifo, to_write, sizeof(*to_write));
}
