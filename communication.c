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

void read_fifo_answer(char *path)
{
    tlv_reply_t t;

    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    read(fifo, &t, sizeof(t));
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

    read(fifo, t, sizeof(*t));
    close(fifo);

    printf("passread \n", t->value.create.account_id);
}

void write_fifo_server(char *path, tlv_request_t *to_write)
{
    int fifo = open(path, O_WRONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY);
    }

    write(fifo, to_write, sizeof(*to_write));
    close(fifo);
}

//TODO: update to right struct
void write_fifo_answer(char *path, tlv_reply_t *to_write)
{
    int fifo = open(path, O_WRONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY);
    }

    write(fifo, to_write, sizeof(*to_write));
    close(fifo);
}
