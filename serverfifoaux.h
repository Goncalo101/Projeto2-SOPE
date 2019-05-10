#ifndef _SERVERFIFOAUX_H_
#define _SERVERFIFOAUX_H_

#include "types.h"
#include "userflag.h"

void create_header_struct(User_flag flag, pid_t pid, req_header_t* t);
void create_new_account_struct(User_flag flag, req_create_account_t* create);
void create_transfer_struct(User_flag flag, req_transfer_t* transf);
void create_tlv_request_struct(tlv_request_t* tlv, User_flag flag, req_transfer_t* transf, req_create_account_t* create, req_header_t* header);
tlv_request_t join_structs_to_send(User_flag flag);

#endif /* _SERVERFIFOAUX_H_ */