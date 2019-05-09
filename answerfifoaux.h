#ifndef _ANSWERFIFOAUX_H_
#define _ANSWERFIFOAUX_H_

#include "types.h"

void create_header_struct_a(int id, int ret_code, rep_header_t *t);
void create_balance_struct_a(int balance, rep_balance_t *create);
void create_transfer_struct_a(int balance, rep_transfer_t *create);
void create_shutdown_struct_a(int active, rep_shutdown_t *shutd);
void create_tlv_reply_struct(tlv_reply_t *tlv, int opnumber,
                               rep_header_t *header,
                               rep_balance_t *balance,
                               rep_transfer_t *transfer,
                               rep_shutdown_t *shutdown);

tlv_reply_t join_structs_to_send_a(int opnumber,rep_header_t *header,
                               rep_balance_t *balance,
                               rep_transfer_t *transfer,
                               rep_shutdown_t *shutdown);

#endif /* _ANSWERFIFOAUX_H_ */