#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

// int readline(int fd, char *str)
// {
//     int n;
//     do
//     {
//         n = read(fd, str, 1);
//     } while (n > 0 && *str++ != '\0');
//     return (n > 0);
// }

void read_fifo_answer(char *path, tlv_reply_t *t)
{
    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    read(fifo, &(t->type), sizeof(op_type_t));
    read(fifo, &(t->length), sizeof(uint32_t));
    read(fifo, &(t->value), sizeof(rep_value_t));
    // read(fifo, t, sizeof(tlv_reply_t));
    close(fifo);
}

void read_fifo_server(char *path, tlv_request_t *t)
{
    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    read(fifo, &(t->type), sizeof(op_type_t));
    read(fifo, &(t->length), sizeof(uint32_t));
    read(fifo, &(t->value), sizeof(req_value_t));
    // read(fifo, t, sizeof(tlv_request_t));
    close(fifo);

    printf("id %d \n", t->value.create.account_id);
    printf("type %d \n", t->type);
    printf("lenght %d \n", t->length);
}

void write_fifo_server(char *path, tlv_request_t *to_write)
{
    int fifo = open(path, O_WRONLY | O_TRUNC);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY | O_TRUNC);
    }

    write(fifo, &(to_write->type), sizeof(op_type_t));
    write(fifo, &(to_write->length), sizeof(uint32_t));
    write(fifo, &(to_write->value), sizeof(req_value_t));
    // write(fifo, to_write, sizeof(tlv_request_t));
    close(fifo);
}

//TODO: update to right struct
void write_fifo_answer(char *path, tlv_reply_t *to_write)
{
    int fifo = open(path, O_WRONLY | O_TRUNC);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY | O_TRUNC);
    }

    write(fifo, &(to_write->type), sizeof(op_type_t));
    write(fifo, &(to_write->length), sizeof(uint32_t));
    write(fifo, &(to_write->value), sizeof(rep_value_t));
    // write(fifo, to_write, sizeof(tlv_reply_t));
    close(fifo);
}
