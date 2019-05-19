#include "accounts.h"

#include "crypto.h"
#include "sope.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static uint32_t account_ids[MAX_BANK_ACCOUNTS] = {0};
static pthread_mutex_t account_mutexes[MAX_BANK_ACCOUNTS] = PTHREAD_MUTEX_INITIALIZER;
static uint32_t active_threads = 0;
static pthread_mutex_t active_thrds = PTHREAD_MUTEX_INITIALIZER;

void change_active(int fildes, int number_office, int flag)
{
    pthread_mutex_lock(&active_thrds);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, 0);
    if (flag == ADD_ACTIVE_THREAD)
        active_threads++;
    else if (flag == REMOVE_ACTIVE_THREAD)
        active_threads--;
    pthread_mutex_unlock(&active_thrds);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, 0);
}
void insert_account(bank_account_t account)
{
    accounts[account.account_id] = account;
    account_ids[account.account_id] = 1;
}

void create_admin_account(char *password, int fildes)
{
    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash);

    bank_account_t account;

    account.account_id = 0;
    account.balance = 0;
    strcpy(account.salt, salt);
    strcpy(account.hash, hash);

    insert_account(account);

    logAccountCreation(fildes, 0, &account);
}

ret_code_t create_account(char *password, uint32_t balance, uint32_t new_id, uint32_t account_create_id, uint32_t delay, int fildes, int number_office)
{

    bank_account_t account;

    if (account_create_id != 0)
    {
        return RC_OP_NALLOW;

    }

    pthread_mutex_lock(&account_mutexes[new_id]);
    op_delay(delay, 0, fildes);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, new_id);

    char salt[SALT_LEN + 1];
    create_salt(salt);
    char hash[HASH_LEN + 1];
    create_hash(password, salt, hash);
    hash[HASH_LEN] = '\0';

    if (account_ids[new_id] == 1)
        return RC_ID_IN_USE;

    account.account_id = new_id;
    account.balance = balance;
    strcpy(account.hash, hash);
    strcpy(account.salt, salt);

    insert_account(account);

    pthread_mutex_unlock(&account_mutexes[new_id]);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, new_id);

    logAccountCreation(fildes, number_office, &account);

    return RC_OK;
}

ret_code_t transfer_money(uint32_t sender_id, uint32_t receiver_id, uint32_t value, uint32_t delay, int fildes, int number_office, uint32_t *balance)
{
    // check if accounts are the same
    if (sender_id == receiver_id)
    {
        return RC_SAME_ID;
    }

    pthread_mutex_lock(&account_mutexes[sender_id]);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, sender_id);
    pthread_mutex_lock(&account_mutexes[receiver_id]);
    logSyncMech(fildes, number_office, SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, receiver_id);
    op_delay(delay, number_office, fildes);

    // check if either of the accounts doesn't exist (the sender has to exist so it might not be
    // necessary to check if the sender exists)
    if (account_ids[sender_id] == 0 || account_ids[receiver_id] == 0)
    {
        pthread_mutex_unlock(&account_mutexes[sender_id]);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, sender_id);
        return RC_ID_NOT_FOUND;
    }

    // check if sender's balance would be too low
    if (MIN_BALANCE < accounts[sender_id].balance + value)
    {
        pthread_mutex_unlock(&account_mutexes[sender_id]);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, sender_id);
        pthread_mutex_unlock(&account_mutexes[receiver_id]);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, receiver_id);
        return RC_NO_FUNDS;
    }

    // check if receiver's balance would be too high
    if (accounts[sender_id].balance + value > MAX_BALANCE)
    {
        return RC_TOO_HIGH;
        pthread_mutex_unlock(&account_mutexes[sender_id]);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, sender_id);
        pthread_mutex_unlock(&account_mutexes[receiver_id]);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, receiver_id);
    }

    accounts[sender_id].balance -= value;
    balance = accounts[sender_id].balance;

    return RC_OK;
}

ret_code_t authenticate_user(uint32_t id, uint32_t delay, char *password, int fildes, int number_office)
{
    char hash[HASH_LEN + 1];
    hash[HASH_LEN] = '\0';

    pthread_mutex_lock(&account_mutexes[id]);
    op_delay(delay, number_office, fildes);
    if (account_ids[id] != 1)
    {
        pthread_mutex_unlock(&account_mutexes[id]);
        return RC_LOGIN_FAIL;
    }

    create_hash(password, accounts[id].salt, hash);

    if (strcmp(hash, accounts[id].hash) == 0)
    {
        pthread_mutex_unlock(&account_mutexes[id]);
        return RC_OK;
    }
    else
    {
        pthread_mutex_unlock(&account_mutexes[id]);
        return RC_LOGIN_FAIL;
    }
}

ret_code_t get_account(uint32_t account_id, bank_account_t *account)
{
    if (account_ids[account_id] != 1)
        return RC_OTHER;
    else
    {
        *account = accounts[account_id];
        return RC_OK;
    }
}

void op_delay(uint32_t delayMS, int threadID, int fildes)
{
    usleep(delayMS * 1000);
    logDelay(fildes, threadID, delayMS);
}

ret_code_t handle_balance_request(uint32_t delay, uint32_t id, uint32_t *balance, int fildes, int number_office)
{
    if (id != ADMIN_ACCOUNT_ID)
    {
        bank_account_t account;

        pthread_mutex_lock(&account_mutexes[id]);
        op_delay(delay, number_office, fildes);
        ret_code_t ret = get_account(id, &account);
        pthread_mutex_unlock(&account_mutexes[id]);

        if (ret == RC_OTHER)
            return RC_OTHER;
        *balance = account.balance;
        return RC_OK;
    }
    else
    {
        return RC_OP_NALLOW;
    }
}

ret_code_t handle_shutdown(uint32_t id, uint32_t *shutdown, uint32_t *active_nbr, uint32_t delay, int fildes, int number_office)
{
    printf("sutdown id %d\n", id);
    if (id == 0)
    {
        printf("oi\n");
        *shutdown = 1;
        pthread_mutex_lock(&active_thrds);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_LOCK, SYNC_ROLE_ACCOUNT, 0);
        *active_nbr = active_threads;
        pthread_mutex_unlock(&active_thrds);
        logSyncMech(fildes, number_office, SYNC_OP_MUTEX_UNLOCK, SYNC_ROLE_ACCOUNT, 0);

        op_delay(delay, 0, fildes);
        chmod(SERVER_FIFO_PATH, 0444);
        return RC_OK;
    }
    else
        return RC_OP_NALLOW;
}