#include "userflag.h"
#include "types.h"
#include "define.h"

#include <stdio.h>
#include <stdlib.h>

/* verifications to arguments*/
//-----------------------------------------------------------------------------------
int verify_id(int id){ return (id < MAX_BANK_ACCOUNTS && id >= 1);     }

int verify_pass_len(char *pass)
{
    int l = strlen(pass);
    return (l < MAX_PASSWORD_LEN && l > MIN_PASSWORD_LEN);
}

int verify_opnumber(int opnumber) { return (opnumber <= 4 && opnumber > -1);}

int verify_balance(int balance){  return (balance > MIN_BALANCE && balance < MAX_BALANCE);}

int verify_new_account_args(char *arguments)
{
    int id, balance;
    id = balance = 0;
    char password[100 + 1];
    sscanf(arguments, "%d %d %s", &id, &balance, password);


    return (verify_balance(balance) && verify_id(id) && verify_pass_len(password));
}
//-----------------------------------------------------------------------------------

int addflag(char *argv[], User_flag *flag)
{
    if (verify_id(flag->id) == 0 && verify_pass_len(flag->password) && verify_opnumber(flag->opnumber))
        return WRONG_ARGUMENTS;

    flag->id = atoi(argv[1]);
    strncpy(flag->password, argv[2], sizeof(flag->password));
    flag->delay = atoi(argv[3]);
    flag->opnumber = atoi(argv[4]);
    strncpy(flag->arguments, argv[5], sizeof(flag->arguments));


    if (flag->opnumber == OP_CREATE_ACCOUNT)
    {
        if(verify_new_account_args(flag->arguments) == 0)
        {
            return WRONG_ARGUMENTS;
        }
    }
    

return 0;
}

void printTest(User_flag flag)
{
    printf("id %d \n", flag.id);
    printf("pass %s \n", flag.password);
    printf("delay %d \n", flag.delay);
    printf("opnumber %d \n", flag.opnumber);
    printf("arguments %s \n", flag.arguments);
}