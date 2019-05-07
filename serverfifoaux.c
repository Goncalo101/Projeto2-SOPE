#include "serverfifoaux.h"

#include <stdio.h>


void create_header_struct(User_flag flag, pid_t pid, req_header_t* t)
{
    t->account_id = flag.id;
    t->pid = pid;
    strncpy(t->password, flag.password, sizeof(t->password));
    t->op_delay_ms = flag.delay;

}

void create_new_account_struct(User_flag flag, req_create_account_t *create)
{
    int id = 0;
    int balance  = 0;
    char password[MAX_PASSWORD_LEN + 1];

    sscanf(flag.arguments, "%d %d %s", &id, &balance, password);
    create->account_id = id;
    create->balance = balance;
    strncpy(create->password, password, sizeof(create->password));

    printf("id %d \n", create->account_id);
    printf("balance %d \n", create->balance);
    printf("pass %s \n", create->password);

}

void create_transfer_struct(User_flag flag,req_transfer_t *transf)
{
    int id = 0;
    int amount  = 0;

    sscanf(flag.arguments, "%d %d", &id, &amount);
    transf->account_id = id;
    transf->amount = amount;

    // printf("id %d \n", transf.account_id);
    // printf("amount %d \n", transf.amount);

}

void create_tlv_request_struct(tlv_request_t *tlv,User_flag flag,req_transfer_t *transf,req_create_account_t *create, req_header_t *header)
{
    req_value_t value; 

    value.header = *header;
    if(transf != NULL)
        value.transfer = *transf;
    if(create != NULL)
        value.create = *create;

    tlv->length = sizeof(value);
    tlv->type = flag.opnumber;
    tlv->value = value;

}

tlv_request_t join_structs_to_send(User_flag flag)
{
    req_header_t header;
    req_create_account_t account;
    req_transfer_t transfer;
    tlv_request_t tlv;

    create_header_struct(flag, getpid(),&header);

    if(flag.opnumber == 0)
    {
        create_new_account_struct(flag, &account);
        create_tlv_request_struct(&tlv,flag,NULL,&account,&header);
    }
    else if(flag.opnumber == 2)
    {
        create_transfer_struct(flag, &transfer);
        create_tlv_request_struct(&tlv,flag,&transfer,NULL,&header);
    }
    else
    {
        create_tlv_request_struct(&tlv,flag,NULL,NULL,&header);
    }
    

    return tlv;
}



