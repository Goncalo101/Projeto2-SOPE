#include "constants.h"
#include "types.h"
#include <fcntl.h>
#include "info.h"



void processInformation(req_header_t *t)
{
    t->pid = getpid();

}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Wrong Usage: user <id> <password> <delay> <operation nr> <list of arguments> \n");
        exit(1);
    }

    Info *info = addInfo(argv);
    printTest(info);

    /*
    int fd = open(SERVER_FIFO_PATH, O_WRONLY);
    if(fd == -1)
    {
        printf("No fifo open \n");
        exit(1);
    }
*/
    return 0;
}