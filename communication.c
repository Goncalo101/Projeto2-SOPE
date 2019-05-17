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

void read_fifo_answer(char *name, tlv_reply_t *t)
{
    int fifo_answer_read = open(name, O_RDONLY);
    read(fifo_answer_read, t, sizeof(tlv_reply_t));
    close(fifo_answer_read);
}

void read_fifo_server(tlv_request_t *t)
{
    int fifo_server_read = open(SERVER_FIFO_PATH, O_RDONLY);

    read(fifo_server_read, t, sizeof(tlv_request_t));
    close(fifo_server_read);
}

ret_code_t write_fifo_server(tlv_request_t *to_write)
{
    int fifo_server_write = open(SERVER_FIFO_PATH, O_WRONLY);
    
    if (fifo_server_write == -1) 
       return RC_SRV_DOWN;

    write(fifo_server_write, to_write, sizeof(tlv_request_t));
    close(fifo_server_write);

    return RC_OK;
}

void write_fifo_answer(char *name, tlv_reply_t *to_write)
{
    int fifo_answer_write = open(name, O_WRONLY);

    if (fifo_answer_write == -1) 
       to_write->value.header.ret_code = RC_USR_DOWN;

    write(fifo_answer_write, to_write, sizeof(tlv_reply_t));
    close(fifo_answer_write);
}
