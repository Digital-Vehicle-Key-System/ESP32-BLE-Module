
#ifndef APPLICATION_H
#define APPLICATION_H

#include "crypto.h"

extern unsigned char check;


extern char *mobile_pub_key ;
extern char *mobile_priv_key ;

extern char *vehicle_pub_key ;
extern char *vehicle_priv_key;


extern unsigned char *encrypt_message;
extern char *decrypted_message;
extern char *challenge;
extern size_t encrypttext__len ;

extern unsigned char AES_key[AES_KEYLEN];
extern unsigned char AES_iv[AES_BLOCK_SIZE];

extern unsigned char ciphertext[128] ;
extern unsigned char decryptedtext[128] ;

extern int decryptedtext_len;

enum VehicleAuthState{
    IDLE,
    ReceiveMobilePublicKey,
    SendPublicKey,
    SendChallenge,
    ReceiveChallenge,
    SendAESkey,
    SendAESIV,
    WaitForRequest,
    ExitLoop

};

void authentication_process();

#endif