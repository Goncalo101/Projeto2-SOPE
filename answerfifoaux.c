#include "answerfifoaux.h"

#include <stdio.h>

void create_header_struct_a(uint32_t id, ret_code_t ret_code, rep_header_t* t)
{
    t->account_id = id;
    t->ret_code = ret_code;
}

void create_balance_struct_a(uint32_t balance, rep_balance_t* create)
{
    create->balance = balance;
}

void create_transfer_struct_a(uint32_t balance, rep_transfer_t* create)
{
    create->balance = balance;
}

void create_shutdown_struct_a(uint32_t active, rep_shutdown_t* shutd)
{
    shutd->active_offices = active;
}

void create_tlv_reply_struct(tlv_reply_t* tlv, uint32_t opnumber,
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

tlv_reply_t join_structs_to_send_a(uint32_t opnumber, rep_header_t* header,
    rep_balance_t* balance,
    rep_transfer_t* transfer,
    rep_shutdown_t* shutdown)
{
    tlv_reply_t tlv;
    create_tlv_reply_struct(&tlv, opnumber, header, balance, transfer, shutdown);

    return tlv;
}
