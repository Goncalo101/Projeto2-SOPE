#include "communication.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void create_name_fifo(char *final, pid_t pid)
{
    char c_pid[5];
    strcpy(final, USER_FIFO_PATH_PREFIX);
    sprintf(c_pid, "%d", pid);
    strcat(final, c_pid);
}

void read_fifo_answer(char *path, tlv_reply_t *t)
{
    int fifo = open(path, O_RDONLY);
    while (fifo == -1)
    {
        sleep(1);
        fifo = open(path, O_RDONLY);
    }

    read(fifo, t, sizeof(*t));
    close(fifo);
}

void read_fifo_server(int fifo, tlv_request_t *t)
{
    printf("fd read: %d\n", fifo);
    int r = 0;
    // int w = write(fifo + 1, t, sizeof(tlv_request_t));
    r = read(fifo, t, sizeof(tlv_request_t));
    printf("after read\n");
    // while (r == 0)
    // {
    //     r = read(fifo, t, sizeof(tlv_request_t));
        
    //     if (r == -1)
    //         perror("");
    // }
}

void write_fifo_server(int fifo, tlv_request_t *to_write)
{
    printf("fd write: %d\n", fifo);

    int w = write(fifo, to_write, sizeof(tlv_request_t));
    printf("write return: %d \n", w);
}

//TODO: update to right struct
void write_fifo_answer(int fifo, tlv_reply_t *to_write)
{
    write(fifo, to_write, sizeof(*to_write));
}
