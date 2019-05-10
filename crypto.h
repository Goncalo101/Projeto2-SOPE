#ifndef _CRYPTO_H_
#define _CRYPTO_H_

void create_salt(char* salt);
void create_hash(char* pass, char* salt, char* hash);
/**
 * @brief Computes the sha256sum of a file given its name/path to it
 * 
 * @param file_name name/path of the file to be analyzed
 * @param result string containing the sha256sum of the analyzed file
 * @param flg useful for the logger
 */
void sha256(const char* file_name, char* result);

#endif
