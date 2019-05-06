#ifndef _INFO_H_
#define _INFO_H_

#include <string.h>
#include "constants.h"

typedef struct {
    int id;
    char password[MAX_PASSWORD_LEN +1];
    int delay;
    int opnumber;
    char arguments[BUFFER_SIZE];

} Info;

Info addInfo( char *argv[]);
void printTest(Info info);


#endif /* _INFO_H_ */