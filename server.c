#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include "constants.h"
#include "types.h"
#include "accounts.h"

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

    if (argc != 3)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    create_account(argv[2], "test", 0);

    //TODO: add balconies

    //create fifo to send information
    mkfifo(SERVER_FIFO_PATH, 0660);

    //opens fifo
    int fifo_server;
    char str[100];

    fifo_server = open(SERVER_FIFO_PATH, O_RDONLY);
    while (fifo_server == -1)
    {
        sleep(1);
    }

    readline(fifo_server, str);
        printf("%s\n", str);

   /* char fifopid[USER_FIFO_PATH_LEN];
    strcat(fifopid, USER_FIFO_PATH_PREFIX);
    strcat(fifopid, "20000");*/

    int fifo_answer = open(USER_FIFO_PATH_PREFIX, O_WRONLY);
    while(fifo_answer ==-1)
    {
        printf("meias\n");
    }


    char message[100];
    int messagelen;
    sprintf(message, "Goodbye\n");
    messagelen = strlen(message) + 1;
    write(fifo_answer, message, messagelen);

    return 0;
}