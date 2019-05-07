#include "accounts.h"

#include <string.h>
#include <stdio.h>

static uint32_t id = 0;

bank_account_t accounts[MAX_BANK_ACCOUNTS];

void insert_account(bank_account_t account)
{
    accounts[account.account_id] = account;
}

//TODO: add possibility to be differrent id and not incremented automatically
//Need to verify if it was already used
bank_account_t create_account(char *password, char *salt, int balance) 
{

    bank_account_t account;
    account.account_id = id++;
    account.balance = balance;
    strcpy(account.salt, salt);
    strcpy(account.hash, password);

    insert_account(account);

    return account;
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