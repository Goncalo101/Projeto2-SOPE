#include "argcheck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_offices(char *number)
{
    return atoi(number) > MAX_BANK_OFFICES && atoi(number) <= 0;
}

int check_server_arguments(char *number, char *pass)
{
    return check_offices(number) && verify_pass_len(pass);
}

int verify_id(uint32_t id) { return (id < MAX_BANK_ACCOUNTS && id >= 1); }

int verify_pass_len(char *pass)
{
    if (strchr(pass, ' ') != NULL)
        return 0;

    size_t l = strlen(pass);
    return (l < MAX_PASSWORD_LEN && l > MIN_PASSWORD_LEN);
}

int verify_opnumber(uint32_t opnumber) { return (opnumber <= 4); } //dont need to check for >= 0 because uint ??

int verify_balance(uint32_t balance) { return (balance > MIN_BALANCE && balance < MAX_BALANCE); }

int verify_transfer_arguments(char *arguments)
{
    uint32_t id_dest, amount;
    id_dest = amount = 0;
    sscanf(arguments, "%u %u", &id_dest, &amount);

    return (verify_balance(amount) && verify_id(id_dest));
}

int verify_new_account_args(char *arguments)
{
    uint32_t id, balance;
    id = balance = 0;
    char password[100 + 1];
    sscanf(arguments, "%u %u %s", &id, &balance, password);

    return (verify_balance(balance) && verify_id(id) && verify_pass_len(password));
}