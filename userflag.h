#ifndef _USERFLAG_H_
#define _USERFLAG_H_

#include <string.h>
#include "constants.h"

typedef struct {
    int id;
    char password[MAX_PASSWORD_LEN +1];
    int delay;
    int opnumber;
    char arguments[BUFFER_SIZE];

} User_flag;

User_flag addflag( char *argv[]);
void printTest(User_flag flag);


#endif /* _USERFLAG_H_ */