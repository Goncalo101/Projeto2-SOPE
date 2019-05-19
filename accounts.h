#ifndef _ACCOUNTS_H_
#define _ACCOUNTS_H_

#include "types.h"

bank_account_t accounts[MAX_BANK_ACCOUNTS];

void change_active(int fildes, int number_office, int flag);

ret_code_t create_account(char* password, uint32_t balance, uint32_t new_id, uint32_t account_create_id, uint32_t delay, int fildes, int nbr);
ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value, uint32_t delay, int fildes, int number_office, uint32_t* balance);
void insert_account(bank_account_t account);
void show_bank_account(uint32_t id);
ret_code_t get_account(uint32_t account_id, bank_account_t* account);
void op_delay(uint32_t delayMS, int threadID, int fildes);
ret_code_t handle_balance_request(uint32_t delay, uint32_t id, uint32_t* balance, int fildes, int nbr);
ret_code_t handle_shutdown(uint32_t id, uint32_t* shutdown, uint32_t* active_nbr, uint32_t delay, int fildes, int number_office);
void create_admin_account(char* password, int fildes);

ret_code_t authenticate_user(uint32_t id, uint32_t delay, char* password, int fildes, int nbr);

#endif /* _ACCOUNTS_H_ */