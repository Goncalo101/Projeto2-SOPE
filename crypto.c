#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "crypto.h"
#include "types.h"

/*generate salt*/
//--------------------------------------------------------------------------------------
static const char alphanum[] = "0123456789"
                               "abcdef";

int stringLength = sizeof(alphanum) - 1;

char genRandom() // Random string generator function.
{
    return alphanum[rand() % stringLength];
}

void create_salt(char* salt)
{
    srand(time(NULL));

    for (int i = 0; i < SALT_LEN; i++)
        salt[i] = genRandom();

    salt[SALT_LEN] = '\0';
}

//--------------------------------------------------------------------------------------

void create_hash(char* pass, char* salt, char* hash)
{
    char tohash[MAX_PASSWORD_LEN + SALT_LEN + 1];
    strcpy(tohash, pass);
    strncat(tohash, salt, strlen(salt));
    sha256(tohash, hash);
}

void sha256(const char* file_name, char* result)
{
    int fd_in[2];
    int fd_out[2];
    pid_t pid;

    pipe(fd_in);
    pipe(fd_out);

    pid = fork();

    if (pid == 0) {
        close(fd_out[READ]);
        close(fd_in[WRITE]);
        dup2(fd_out[WRITE], STDOUT_FILENO);
        dup2(fd_in[READ], STDIN_FILENO);

        execlp("sha256sum", "sha256sum", NULL);

    } else {
        close(fd_out[WRITE]);
        close(fd_in[READ]);
        memset(result, 0, HASH_LEN * sizeof(char));
        
        write(fd_in[WRITE], file_name, strlen(file_name));
        close(fd_in[WRITE]);

        read(fd_out[READ], result, HASH_LEN * sizeof(char));
    }
}