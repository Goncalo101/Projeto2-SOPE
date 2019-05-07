#ifndef _SERVERFIFOAUX_H_
#define _SERVERFIFOAUX_H_


#include "types.h"
#include "userflag.h"

req_header_t create_header_struct(User_flag flag, pid_t pid);
req_create_account_t create_new_account_struct(User_flag flag);


#endif /* _SERVERFIFOAUX_H_ */