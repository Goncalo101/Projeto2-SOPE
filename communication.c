#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int readline(int fd, char *str)
{
    int n;
    do
    {
        n = read(fd, str, 1);
    } while (n > 0 && *str++ != '\0');
    return (n > 0);
}

void read_fifo_answer(char *path)
{
    char str[100];

    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    readline(fifo, str);
    printf("%s\n", str);
}

void read_fifo_server(char *path)
{
    char str[100];
    tlv_request_t *t;

    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    read(fifo, t, sizeof(t));

    printf("%d", t->length);

    //readline(fifo, str);
    //printf("%s\n", str);
}

void write_fifo_server(char *path, tlv_request_t *to_write)
{
    tlv_request_t *message;
    int messagelen;

    int fifo = open(path, O_WRONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY);
    }

    // sprintf(message, to_write, "%s");
    // messagelen = strlen(message) + 1;
    write(fifo, to_write, sizeof(to_write));
}

//TODO: update to right struct
void write_fifo_answer(char *path, char *to_write)
{
    char message[100];
    int messagelen;

    int fifo = open(path, O_WRONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_WRONLY);
    }

    sprintf(message, to_write, "%s");
    messagelen = strlen(message) + 1;
    write(fifo, message, messagelen);
}
