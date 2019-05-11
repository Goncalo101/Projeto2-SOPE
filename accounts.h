#ifndef _ACCOUNTS_H_
#define _ACCOUNTS_H_

#include "types.h"

bank_account_t accounts[MAX_BANK_ACCOUNTS];
int account_ids[MAX_BANK_ACCOUNTS];

ret_code_t create_account(char* password, int balance, int new_id, int account_create_id);
ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value);
void insert_account(bank_account_t account);
void show_bank_account(int id);
bank_account_t* get_account(uint32_t account_id);
void op_delay(int delayMS);
ret_code_t handle_balance_request(int delay, int id, int* balance);
ret_code_t handle_shutdown(int id, int *shutdown, int *active_nbr);
void create_admin_account(char* password);
int get_account_index(uint32_t account_id);
ret_code_t authenticate_user(req_header_t req_header);

#endif /* _ACCOUNTS_H_ */