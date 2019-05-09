#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "crypto.h"
#include "define.h"
#include "types.h"

/*generate salt*/
//--------------------------------------------------------------------------------------
static const char alphanum[] =
"0123456789"
"abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()  // Random string generator function.
{
    return alphanum[rand() % stringLength];
}

void create_salt(char*salt)
{
    srand(time(NULL));  
    
    for(int i = 0 ;i < SALT_LEN;i++)
        salt[i] = genRandom();

    printf("salt %s \n", salt);

}

//--------------------------------------------------------------------------------------

void create_hash(char*pass, char*salt, char*hash)
{
    char tohash[BUFFER_SIZE];
    strncat(tohash,pass, strlen(pass));
    strncat(tohash,salt, strlen(salt));

    sha256(tohash,hash);

}


void sha256(const char* file_name, char* result)
{
    int fd[2];
    pid_t pid;

    pipe(fd);

    pid = fork();

    if (pid == 0) {
        dup2(fd[WRITE], STDOUT_FILENO);
        close(fd[READ]);

        char program_name[] = "sha256sum";

        execlp(program_name, program_name, file_name, NULL);
    } else {
        close(fd[WRITE]);
        memset(result, 0, BUFFER_SIZE * sizeof(char));
        read(fd[READ], result, SHA256_SIZE * sizeof(char));
    }
}