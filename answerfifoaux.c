#include "answerfifoaux.h"

#include <stdio.h>

void create_header_struct_a(int id, int ret_code, rep_header_t* t)
{
    t->account_id = id;
    t->ret_code = ret_code;
}

void create_balance_struct_a(int balance, rep_balance_t* create)
{
    create->balance = balance;
}

void create_transfer_struct_a(int balance, rep_transfer_t* create)
{
    create->balance = balance;
}

void create_shutdown_struct_a(int active, rep_shutdown_t* shutd)
{
    shutd->active_offices = active;
}

void create_tlv_reply_struct(tlv_reply_t* tlv, int opnumber,
    rep_header_t* header,
    rep_balance_t* balance,
    rep_transfer_t* transfer,
    rep_shutdown_t* shutdown)
{
    rep_value_t value;
    value.header = *header;

    if (balance != NULL)
        value.balance = *balance;
    if (transfer != NULL)
        value.transfer = *transfer;
    if (shutdown != NULL)
        value.shutdown = *shutdown;

    tlv->value = value;
    tlv->length = sizeof(value);
    tlv->type = opnumber;
}

tlv_reply_t join_structs_to_send_a(int opnumber, rep_header_t* header,
    rep_balance_t* balance,
    rep_transfer_t* transfer,
    rep_shutdown_t* shutdown)
//TODO: add struct with values from the operations to use on the right places and respective return values?
{
    tlv_reply_t tlv;
    create_tlv_reply_struct(&tlv, opnumber, header, balance, transfer, shutdown);

    return tlv;
}
