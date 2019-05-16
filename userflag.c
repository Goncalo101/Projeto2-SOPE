#include "userflag.h"
#include "define.h"
#include "types.h"
#include "argcheck.h"

#include <stdio.h>
#include <stdlib.h>

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