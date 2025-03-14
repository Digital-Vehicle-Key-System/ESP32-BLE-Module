#ifndef CRYPTO_H
#define CRYPTO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bio.h>



#define CHALLENGE_BYTES 16        // Number of random bytes for challenge (16 bytes = 128 bits)
#define AES_KEYLEN     32    // 32 bytes = 256 bits for AES-256
#define AES_BLOCK_SIZE 16    // AES block size is 16 bytes





void generate_keys(char **public_key, char **private_key);
unsigned char *encrypt_message_to_array(const char *plaintext, const char *pubkey_data,size_t *ciphertext_len);
char *decrypt_message(const char *private_key_pem,  const unsigned char *encrypted,size_t encrypted_len);

int generate_aes_key(unsigned char *key, unsigned char *iv, int key_len) ;
int aes_encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv,unsigned char *ciphertext);
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,const unsigned char *key, const unsigned char *iv,unsigned char *plaintext);


int save_bytes_to_file(const char *filename, const unsigned char *data, size_t data_len);
int read_bytes_from_file(const char *filename, unsigned char *data, size_t data_len);
void print_hex(const char *label, const unsigned char *data, int len) ;


char *generate_random_challenge(int num_bytes);
int compare_challenge(const char *original, const char *decrypted) ;
#endif