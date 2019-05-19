#include "userflag.h"
#include "argcheck.h"
#include "types.h"

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
