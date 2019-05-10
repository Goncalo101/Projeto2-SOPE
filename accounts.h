#ifndef _ACCOUNTS_H_
#define _ACCOUNTS_H_

#include "types.h"

bank_account_t accounts[MAX_BANK_ACCOUNTS];
int account_ids[MAX_BANK_ACCOUNTS];

ret_code_t create_account(char *password, int balance, int new_id, int account_create_id);
ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value) ;
void insert_account(bank_account_t account);
void show_bank_account(int id);
int getHash(char* password, char* salt, char* hash);
bank_account_t* getAccount(uint32_t account_id);
void opDelay(int delayMS, int threadID);
tlv_reply_t newReply();
tlv_reply_t makeErrorReply(int retCode, tlv_reply_t request);
tlv_reply_t makeBalanceReply(int accountId, int balance);
tlv_reply_t handleBalanceRequest(tlv_request_t request, int threadID);


#endif /* _ACCOUNTS_H_ */