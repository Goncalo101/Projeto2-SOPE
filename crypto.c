#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "crypto.h"
#include "define.h"
#include "types.h"

#include <errno.h>

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

void create_salt(char* salt)
{
    srand(time(NULL));  
    
    for(int i = 0 ;i < SALT_LEN;i++)
        salt[i] = genRandom();
    
    salt[SALT_LEN]= '\0';

    printf("salt %s \n", salt);

}

//--------------------------------------------------------------------------------------

void create_hash(char*pass, char*salt, char*hash)
{
    char tohash[HASH_LEN + SALT_LEN +1];
    strcpy(tohash,pass);
    strncat(tohash,salt, strlen(salt));
    sha256(tohash,hash);
    printf("hash: %s\n", hash);
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
            
        char command[BUFFER_SIZE];
        sprintf(command, "echo -n %s | sha256sum" , file_name); 
        system(command);

    } else {
        close(fd[WRITE]);
        memset(result, 0, HASH_LEN * sizeof(char));
        read(fd[READ], result, SHA256_SIZE * sizeof(char));
    }
}