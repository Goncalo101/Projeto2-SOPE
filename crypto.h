#ifndef _CRYPTO_H_
#define _CRYPTO_H_


#define READ 0
#define WRITE 1

void create_salt(char* salt);
void create_hash(char* pass, char* salt, char* hash);
void sha256(const char* file_name, char* result);

#endif
