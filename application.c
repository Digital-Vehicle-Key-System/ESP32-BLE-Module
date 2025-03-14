
#include "application.h"

unsigned char AES_key[AES_KEYLEN];
unsigned char AES_iv[AES_BLOCK_SIZE];

unsigned char ciphertext[128] = {0};
unsigned char decryptedtext[128] = {0};

size_t encrypttext__len = 0;
int decryptedtext_len =0;
char *vehicle_pub_key = NULL;
char *vehicle_priv_key = NULL;
char *challenge=NULL;


unsigned char *encrypt_message=NULL;
char *decrypted_message=NULL;

char *mobile_pub_key = NULL;
char *mobile_priv_key = NULL;

unsigned char state=IDLE;
unsigned char ret=0;
unsigned char check=0;


void authentication_process(){
	switch(state){
		case IDLE:
            /*1- Generate the public and private keys*/
            generate_keys(&vehicle_pub_key, &vehicle_priv_key);
            generate_keys(&mobile_pub_key, &mobile_priv_key);/*For test only*/

            /*2- Generate random challenge*/
            challenge = generate_random_challenge(CHALLENGE_BYTES);
            printf("Generated Challenge: %s\n", challenge); 
                         

            /*3- Change state*/
            state = ReceiveMobilePublicKey;
            check++;
		break;
        case ReceiveMobilePublicKey:
            /*1- Receive mobile public key and store it*/

            /*2- Check Access*/

            /*if access succeed save the public key in mobile_pub_key variable*/
            ret=1;
            printf("Mobile Public Key received successfully\n")   ; 

            /*3- Change the state*/
            if(1== ret){
                state=SendPublicKey;
            }
            else{
                state=IDLE;
            }
        break;

        case SendPublicKey:
            /*1- Send Vehicle Public Key*/
            ret=1;
            printf("Vehicle Public Key sent successfully\n")   ;        
            /*2- Change the state*/
            if(1== ret){
                state=SendChallenge;
            }
            else{
                state=IDLE;
            }
        break;


		case SendChallenge:
            
            /*1- Encrypt the challenge with public key of the mobile, and save the ciphertext to an encrypt_message */
            encrypt_message = encrypt_message_to_array(challenge, mobile_pub_key, &encrypttext__len);
            print_hex("RSA encrypt_message send to the mobile: ", encrypt_message,encrypttext__len);
            /*2- Send to mobile app*/
            /************************************for test***********************************************/

            /* mobile will decrypted this challenge with its private key */
            decrypted_message = decrypt_message(mobile_priv_key, encrypt_message,encrypttext__len);
            printf("Decrypted challenge: %s\n", decrypted_message);            
            /* Compare challenge to check equallity */
            ret=compare_challenge(challenge, (char *)decrypted_message);
            free(decrypted_message);
            

            /***********************************************************************************/
            /*3- Change the state*/
            if(1== ret){
                state=ReceiveChallenge;
            }
            else{
                state=IDLE;
            }
           
		break;
		case ReceiveChallenge:
            /*1- Receive decrypted challenge from mobile app*/
            /**************************************for test*********************************************/
            /*Encrypt the challenge with public key of the vehicle, and save the ciphertext to an encrypt_message */

            free(encrypt_message);
            encrypt_message = encrypt_message_to_array(challenge, vehicle_pub_key, &encrypttext__len);
            print_hex("RSA encrypt_message send to vehicle: ", encrypt_message,encrypttext__len);

            /***********************************************************************************/

            /*save the challenge in encrypt_message variable  */

            /*2- Decrypt the message from encrypt_message and print it.*/
            decrypted_message = decrypt_message(vehicle_priv_key, encrypt_message,encrypttext__len);
            printf("Decrypted message: %s\n", decrypted_message);
            
            /*3- Compare challenge to check equallity*/
            ret=compare_challenge(challenge, (char *)decrypted_message);
            free(decrypted_message);
            
            /*4- Change the state*/
            if (1== ret){
                state=SendAESkey;
            }
            else{
                state=IDLE;
            }
           
		break;
		case SendAESkey:
            /*1- Generate the AES-256 key and IV, and save them */
            if (!generate_aes_key(AES_key, AES_iv, AES_KEYLEN)) {
                fprintf(stderr, "Key generation failed.\n");
                ret=0;
            }
            else{
                ret=1;
            }
            /*2- Encrypt the AES key with public key of the mobile, and save the ciphertext to encrypt_message */ 
            free(encrypt_message);               
            encrypt_message = encrypt_message_to_array(AES_key, mobile_pub_key, &encrypttext__len);
            print_hex("AES_key encrypted_message send to mobile: ", encrypt_message,encrypttext__len);
            /*3- Send to the mobile app*/
            /*************************************for test**********************************************/
            /*mobile will Decrypt the message and check it */
            decrypted_message = decrypt_message(mobile_priv_key, encrypt_message,encrypttext__len);
            print_hex("AES key Decrypted message : ", decrypted_message ,strlen(decrypted_message) );
            
            
            ret=compare_challenge(AES_key, (char *)decrypted_message);
            free(decrypted_message);
            /***********************************************************************************/

            /*4- Change the state*/
            if (1== ret){
                state = SendAESIV; 
            }
            else{
                state=IDLE;
            }
              
                   
		break;

        case SendAESIV:

            /*1- Encrypt the AES iv with public key of the mobile, and save the ciphertext to encrypt_message */ 
            free(encrypt_message);               
            encrypt_message = encrypt_message_to_array(AES_iv, mobile_pub_key, &encrypttext__len);
            print_hex("AES_iv encrypted_message sent to mobile: ", encrypt_message,encrypttext__len);
            /*2- Send to the mobile app < encrypt_message >*/
            /************************************for test***********************************************/
            /*mobile will Decrypt the message and check it */
            decrypted_message = decrypt_message(mobile_priv_key, encrypt_message,encrypttext__len);
            print_hex("AES iv Decrypted message : ", decrypted_message ,strlen(decrypted_message) );
            
            
            ret=compare_challenge(AES_iv, (char *)decrypted_message);
            free(decrypted_message);
            /***********************************************************************************/

            /*3- Change the state*/
            if (1== ret){
                state = WaitForRequest; 
            }
            else{
                state=IDLE;
            }
        break;
		case  WaitForRequest:
        /*1- Recieve message from mobile app*/
        /***********************************for test************************************************/
        /*mobile will encrypte challenge with AES key of vehicle */

        encrypttext__len = aes_encrypt((unsigned char *)challenge, strlen(challenge), AES_key, AES_iv, ciphertext);
        if (encrypttext__len == -1) {
            fprintf(stderr, "Encryption failed\n");
            ret= 0;
        }
        else{
            printf("Encrypted Request sent to Vehicle \n");
            ret=1;
        }
        /***********************************************************************************/
        /*2- Decrypt the message from ciphertext using AES key and print it.*/

        decryptedtext_len = aes_decrypt(ciphertext, encrypttext__len, AES_key, AES_iv, decryptedtext);
        if (decryptedtext_len == -1) {
            fprintf(stderr, "Decryption failed\n");
            ret=0 ;
        }
        else{
            decryptedtext[decryptedtext_len] = '\0'; // Null-terminate the decrypted text  
            // Print decrypted text
            printf("Decrypted Request using AES key: %s\n", decryptedtext); 
            ret=1;
            
            
        }
              
  
        /*3- Check the type of the request */

        /*4- Send the request*/

        /*5- change state*/
        state= ExitLoop;
		break;
        
        case ExitLoop :
        printf("number of tries :%d" , check);
       
        check=3;
        break;

		default:
		printf("Undefined State !");
	}
	
}
