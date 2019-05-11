#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void create_name_fifo(char *final, pid_t pid)
{
    char c_pid[5];
    strcpy(final,USER_FIFO_PATH_PREFIX);
    sprintf(c_pid, "%d", pid);
    strcat(final,c_pid);
}

void read_fifo_answer(char *path, tlv_reply_t *t)
{
    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY );
    }


    printf("------------------------------------------- \n");
    printf("id account %d \n", t->value.header.account_id);
    printf("balance transfer account %d \n", t->value.transfer.balance);
    printf("balance %d \n", t->value.balance.balance); 
        printf("------------------------------------------- \n");

    read(fifo, t, sizeof(*t));
    close(fifo);
}

void read_fifo_server(char *path, tlv_request_t *t)
{
    int fifo = open(path, O_RDONLY  );
        printf("fd read: %d\n", fifo);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY );
    }

    int r = read(fifo, t, sizeof(*t));

    printf("read return: %d \n", r);
    close(fifo);

    printf("------------------------------------------- \n");
    printf("id create account %d \n", t->value.create.account_id);
    printf("balance create account %d \n", t->value.create.balance);
    printf("pass create account %s \n", t->value.create.password); 
    printf("type %d \n", t->type);
    printf("lenght %d \n", t->length);
        printf("------------------------------------------- \n");
}

void write_fifo_server(char *path, tlv_request_t *to_write)
{
    int fifo = open(path, O_WRONLY | O_TRUNC );
    printf("fd write: %d\n", fifo);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY | O_TRUNC  );
    }


    int w = write(fifo, to_write, sizeof(*to_write));
    printf("write return: %d \n", w);
    close(fifo);
}

//TODO: update to right struct
void write_fifo_answer(char *path, tlv_reply_t *to_write)
{
    int fifo = open(path, O_WRONLY | O_TRUNC  );
    while (fifo == -1)
    {
        fifo = open(path, O_WRONLY | O_TRUNC);
    }

    write(fifo, to_write, sizeof(*to_write));
    close(fifo);
}
