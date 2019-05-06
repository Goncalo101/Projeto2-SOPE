#include "constants.h"
#include "types.h"
#include <fcntl.h>
#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/*void processInformation(req_header_t *t)
{
    t->pid = getpid();

}*/

int readline(int fd, char *str)
{
    int n;
    do
    {
        n = read(fd, str, 1);
    } while (n > 0 && *str++ != '\0');
    return (n > 0);
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    //handles information from arguments to a struct
    //Info info = addInfo(argv);

    //creates name of fifo to create to read message from server
    /*char fifopid[USER_FIFO_PATH_LEN];
    //char *cpid;
    //sprintf(cpid, "%d", getpid());
    strcat(fifopid, USER_FIFO_PATH_PREFIX);
    strcat(fifopid, "20000");*/


    //creates fifo
    mkfifo(USER_FIFO_PATH_PREFIX, 0660);

    //opens fifo to send message
    int fifo_server = open(SERVER_FIFO_PATH, O_WRONLY);
    if (fifo_server == -1)
    {
        printf("No fifo open \n");
        exit(1);
    }

    char message[100];
    int messagelen;

    sprintf(message, "Hello i am %d\n", getpid());
    messagelen = strlen(message) + 1;
    write(fifo_server, message, messagelen);

    //opens fifo (created by him) to recive answer from server
    char str[100];

    int fifo_answer = open(USER_FIFO_PATH_PREFIX, O_RDONLY);

    while (fifo_answer == -1)
    {
        sleep(1);
    }

    while (readline(fifo_answer, str))
        printf("%s\n", str);

    return 0;
}