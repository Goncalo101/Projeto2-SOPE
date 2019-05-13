#include "userflag.h"
#include "define.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>

/* verifications to arguments*/
//-----------------------------------------------------------------------------------
int verify_id(uint32_t id) { return (id < MAX_BANK_ACCOUNTS && id >= 1); }

int verify_pass_len(char* pass)
{
    size_t l = strlen(pass);
    return (l < MAX_PASSWORD_LEN && l > MIN_PASSWORD_LEN);
}

int verify_opnumber(uint32_t opnumber) { return (opnumber <= 4 && opnumber > -1); }

int verify_balance(uint32_t balance) { return (balance > MIN_BALANCE && balance < MAX_BALANCE); }

int verify_transfer_arguments(char* arguments)
{
    uint32_t id_dest, amount;
    id_dest = amount = 0;
    sscanf(arguments, "%u %u", &id_dest, &amount);

    return (verify_balance(amount) && verify_id(id_dest));
}

int verify_new_account_args(char* arguments)
{
    uint32_t id, balance;
    id = balance = 0;
    char password[100 + 1];
    sscanf(arguments, "%u %u %s", &id, &balance, password);

    return (verify_balance(balance) && verify_id(id) && verify_pass_len(password));
}
//-----------------------------------------------------------------------------------

int addflag(char* argv[], User_flag* flag)
{
    if (verify_id(flag->id) == 0 && verify_pass_len(flag->password) && verify_opnumber(flag->opnumber))
        return WRONG_ARGUMENTS;

    flag->id = atoi(argv[1]);
    strncpy(flag->password, argv[2], sizeof(flag->password));
    flag->delay = atoi(argv[3]);
    flag->opnumber = atoi(argv[4]);
    strncpy(flag->arguments, argv[5], sizeof(flag->arguments));

    switch (flag->opnumber) {
    case OP_CREATE_ACCOUNT: {
        if (verify_new_account_args(flag->arguments) == 0)
            return WRONG_ARGUMENTS;
        break;
    }
    case OP_TRANSFER: {
        if (verify_transfer_arguments(flag->arguments) == 0)
            return WRONG_ARGUMENTS;
        break;
    }
    default:
        break;
    }

    return 0;
}

// void printTest(User_flag flag)
// {
//     printf("id %d \n", flag.id);
//     printf("pass %s \n", flag.password);
//     printf("delay %d \n", flag.delay);
//     printf("opnumber %d \n", flag.opnumber);
//     printf("arguments %s \n", flag.arguments);
// }