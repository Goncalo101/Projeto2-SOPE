#include "accounts.h"

#include <string.h>
#include <stdio.h>

bank_account_t accounts[MAX_BANK_ACCOUNTS];
int account_ids[MAX_BANK_ACCOUNTS];

void insert_account(bank_account_t account)
{
    accounts[account.account_id] = account;
    account_ids[account.account_id] = 1;
}

//TODO: add possibility to be differrent id and not incremented automatically--done
//Need to verify if it was already used
ret_code_t create_account(char *password, char *salt, int balance, int id) 
{
    bank_account_t account;

    if(account_ids[id] == 1)
        return RC_ID_IN_USE;

    account.account_id = id;
    account.balance = balance;
    strcpy(account.salt, salt);
    strcpy(account.hash, password);

    insert_account(account);

    //RC_OTHER
    //RC_OP_NALLOW

    //add log here
    return RC_OK;
}

ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value) 
{
    // check if either of the accounts doesn't exist (the sender has to exist so it might not be
    // necessary to check if the sender exists)
    if (account_ids[sender_id] == 0 || account_ids[receiver_id] == 0) {
        return RC_ID_NOT_FOUND;
    }

    // check if accounts are the same
    if (account_ids[sender_id] == account_ids[receiver_id]) {
        return RC_SAME_ID;
    }

    // check if sender's balance would be too low
    if (accounts[sender_id].balance - value < MIN_BALANCE) {
        return RC_NO_FUNDS;
    }

    // check if receiver's balance would be too high
    if (accounts[sender_id].balance + value > MAX_BALANCE) {
        return RC_TOO_HIGH;
    }

    accounts[sender_id].balance -= value;
    accounts[receiver_id].balance += value;

    return RC_OK;
}


//test
void show_bank_account(int id)
{
    bank_account_t test = accounts[id];
    printf("%d\n", test.account_id);
    printf("%d\n", test.balance);
    printf("%s\n", test.hash);
    printf("%s\n", test.salt);
}
