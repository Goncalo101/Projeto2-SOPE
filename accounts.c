#include "accounts.h"

#include "crypto.h"
#include <stdio.h>
#include <string.h>

void insert_account(bank_account_t account)
{
    accounts[account.account_id] = account;
    account_ids[account.account_id] = 1;
}

//TODO: add possibility to be differrent id and not incremented automatically--done
//Need to verify if it was already used

void create_admin_account(char *password)
{
    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash); //TODO:fix bug in sha256sum

    bank_account_t account;

    account.account_id = 0;
    account.balance = 0;
    strcpy(account.salt, salt);
    strcpy(account.hash, password); //TODO:add hash

    insert_account(account);
    //add log here
}

ret_code_t create_account(char *password, int balance, int new_id, int account_create_id)
{
    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash); //TODO:fix bug in sha256sum

    bank_account_t account;

    if (account_create_id == 0)
        return RC_OP_NALLOW;

    if (account_ids[new_id] == 1)
        return RC_ID_IN_USE;

    account.account_id = new_id;
    account.balance = balance;
    strcpy(account.salt, salt);
    strcpy(account.hash, password); //TODO:add hash

    insert_account(account);

    //RC_OTHER

    //add log here
    return RC_OK;
}

ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value)
{
    // check if either of the accounts doesn't exist (the sender has to exist so it might not be
    // necessary to check if the sender exists)
    if (account_ids[sender_id] == 0 || account_ids[receiver_id] == 0)
    {
        return RC_ID_NOT_FOUND;
    }

    // check if accounts are the same
    if (account_ids[sender_id] == account_ids[receiver_id])
    {
        return RC_SAME_ID;
    }

    // check if sender's balance would be too low
    if (accounts[sender_id].balance - value < MIN_BALANCE)
    {
        return RC_NO_FUNDS;
    }

    // check if receiver's balance would be too high
    if (accounts[sender_id].balance + value > MAX_BALANCE)
    {
        return RC_TOO_HIGH;
    }

    accounts[sender_id].balance -= value;
    accounts[receiver_id].balance += value;

    return RC_OK;
}

int get_account_index(uint32_t account_id){
    for(int i=0;i<accounts.size();i++){
        if(accounts[i].account_id==account_id)
            return i;
    }
    return -1;
}

//handle balance request functions
bank_account_t *get_account(uint32_t account_id)
{
    if (account_ids[account_id] != 1)
        return NULL;
    else
        return &accounts[account_id];
}

void op_delay(int delayMS)
{
    usleep(delayMS * 1000);
}

ret_code_t handle_balance_request(int delay, int id, int *balance)
{
    //opDelay(delay); //TODO:test functionality
    if (id != ADMIN_ACCOUNT_ID)
    {
        bank_account_t *account = get_account(id);
        *balance = account->balance;
        return RC_OK;
    }
    else
    {
        return RC_OP_NALLOW;
    }
}

ret_code_t handle_shutdown(int id, int *shutdown, int *active_nbr)
{
    if (id == 0)
    {
        *shutdown = 1;
        *active_nbr = 1; //TODO:add real number of active threads
        return RC_OK;
    }
    else 
        return RC_OP_NALLOW;
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
