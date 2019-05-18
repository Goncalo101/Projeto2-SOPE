#ifndef _CRYPTO_H_
#define _CRYPTO_H_

void create_salt(char* salt);
void create_hash(char* pass, char* salt, char* hash);
void sha256(const char* file_name, char* result);

#endif
