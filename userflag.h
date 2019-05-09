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

int addflag(char *argv[], User_flag *flag);
void printTest(User_flag flag);

int verify_id(int id);
int verify_pass_len(char *pass);
int verify_opnumber(int opnumber);
int verify_balance(int balance);
int verify_new_account_args(char *arguments);

#endif /* _USERFLAG_H_ */