#ifndef _ARGCHECK_H_
#define _ARGCHECK_H_

#include "types.h"


//COMMON FUNCTIONS
int verify_pass_len(char* pass);

//SERVER FUNCTIONS
int check_offices(char *number);
int check_server_arguments(char *number, char*pass);

//USER FUNCTIONS
int verify_id(uint32_t id);
int verify_opnumber(uint32_t opnumber);
int verify_balance(uint32_t balance);
int verify_transfer_arguments(char* arguments);
int verify_new_account_args(char* arguments);

#endif /* _ARGCHECK_H_ */