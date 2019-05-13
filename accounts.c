#include "accounts.h"

#include "crypto.h"
#include "filedescrp.h"
#include <stdio.h>
#include <string.h>

static uint32_t account_ids[MAX_BANK_ACCOUNTS] = {0};

void insert_account(bank_account_t account)
{
    accounts[account.account_id] = account;
    account_ids[account.account_id] = 1;
}

//TODO: add possibility to be differrent id and not incremented automatically--done
//Need to verify if it was already used

void create_admin_account(char* password)
{
    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash); //TODO:fix bug in sha256sum

    bank_account_t account;

    account.account_id = 0;
    account.balance = 0;
    strcpy(account.salt, salt);
    strcpy(account.hash, hash); //TODO:add hash

    insert_account(account);
    
    //Add log here
    logAccountCreation(getserverfile(), 0, account);
}

ret_code_t create_account(char* password, uint32_t balance, uint32_t new_id, uint32_t account_create_id, uint32_t delay)
{
    op_delay(delay, 0);
    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash); //TODO:fix bug in sha256sum

    bank_account_t account;

    if (account_create_id != 0)
        return RC_OP_NALLOW;

    if (account_ids[new_id] == 1)
        return RC_ID_IN_USE;

    account.account_id = new_id;
    account.balance = balance;
    strcpy(account.salt, salt);
    strcpy(account.hash, hash); //TODO:add hash

    insert_account(account);

    //RC_OTHER

    //Add log here
    logAccountCreation(getuserfile(), 0, account);

    return RC_OK;
}

ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value, uint32_t delay)
{
    op_delay(delay, 0);
    // check if either of the accounts doesn't exist (the sender has to exist so it might not be
    // necessary to check if the sender exists)
    if (account_ids[sender_id] == 0 || account_ids[receiver_id] == 0) {
        return RC_ID_NOT_FOUND;
    }

    // check if accounts are the same
    if (sender_id == receiver_id) {
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

ret_code_t authenticate_user(uint32_t id, uint32_t delay, char* password)
{
    op_delay(delay, 0);
    char hash[HASH_LEN];

    if (account_ids[id] != 1)
        return RC_OTHER;

    create_hash(password, accounts[id].salt, hash);

    if (strcmp(hash, accounts[id].hash) == 0)
        return RC_OK;
    else
        return RC_OTHER;

}

//handle balance request functions
ret_code_t get_account(uint32_t account_id, bank_account_t* account)
{
    if (account_ids[account_id] != 1)
        return RC_OTHER;
    else {
        *account = accounts[account_id];
        return RC_OK;
    }
}

void op_delay(uint32_t delayMS, int threadID)
{
    usleep(delayMS * 1000);
    logDelay(getserverfile(), threadID, delayMS);

}

ret_code_t handle_balance_request(uint32_t delay, uint32_t id, uint32_t* balance)
{
    op_delay(delay,0); //TODO:test functionality
    if (id != ADMIN_ACCOUNT_ID) {
        bank_account_t account;
        ret_code_t ret = get_account(id, &account);

        if (ret == RC_OTHER)
            return RC_OTHER;
        *balance = account.balance;
        return RC_OK;
    } else {
        return RC_OP_NALLOW;
    }
}

ret_code_t handle_shutdown(uint32_t id, uint32_t* shutdown, uint32_t* active_nbr, uint32_t delay)
{
    op_delay(delay,0);
    if (id == 0) {
        *shutdown = 1;
        *active_nbr = 1; //TODO:add real number of active threads
        return RC_OK;
    } else
        return RC_OP_NALLOW;
}