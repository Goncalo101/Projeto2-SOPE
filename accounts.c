#include "accounts.h"

#include <string.h>

static uint32_t id = 0;

bank_account_t create_account(char* password, char* salt, int balance) {
   
    bank_account_t account;
    account.account_id = id++;
    account.balance = balance;
    strcpy(account.salt, salt);
    strcpy(account.hash, password);

    return account; 
}