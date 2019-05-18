#include "communication.h"

#include <errno.h>
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
    alarm(FIFO_TIMEOUT_SECS);
    int fifo_answer_read = open(name, O_RDONLY);

    if (fifo_answer_read == -1 && errno == EINTR)
    {
        t->value.header.ret_code = RC_SRV_TIMEOUT;
    }

    int bytes_read = read(fifo_answer_read, t, sizeof(tlv_reply_t));

    if (bytes_read == -1 && errno == EINTR)
    {
        t->value.header.ret_code = RC_SRV_TIMEOUT;
    }

    close(fifo_answer_read);
}

int read_fifo_server(tlv_request_t *t)
{
    int fifo_server_read = open(SERVER_FIFO_PATH, O_RDONLY);

    int read_srv = read(fifo_server_read, t, sizeof(tlv_request_t));

    close(fifo_server_read);
    return read_srv;
}

ret_code_t write_fifo_server(tlv_request_t *to_write)
{

    int fifo_server_write = open(SERVER_FIFO_PATH, O_WRONLY);

     if (fifo_server_write == -1)
        return RC_USR_DOWN;

    write(fifo_server_write, to_write, sizeof(tlv_request_t));

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
