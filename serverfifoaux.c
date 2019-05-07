#include "serverfifoaux.h"

#include <stdio.h>

req_header_t create_header_struct(User_flag flag, pid_t pid)
{
    req_header_t t;

    t.account_id = flag.id;
    t.pid = pid;
    strncpy(t.password, flag.password, sizeof(t.password));
    t.op_delay_ms = flag.delay;

    return t;
}

req_create_account_t create_new_account_struct(User_flag flag)
{
    req_create_account_t create;
    int id = 0;
    int balance  = 0;
    char password[MAX_PASSWORD_LEN + 1];

    sscanf(flag.arguments, "%d %d %s", &id, &balance, password);
    create.account_id = id;
    create.balance = balance;
    strncpy(create.password, password, sizeof(create.password));

    // printf("id %d \n", create.account_id);
    // printf("balance %d \n", create.balance);
    // printf("pass %s \n", create.password);

    return create;
}


