

#include "application.h"



int main() {
    printf("Hello openssl\n");

     
   

   /* Initialize OpenSSL algorithms and error strings */
   OpenSSL_add_all_algorithms();
   ERR_load_crypto_strings();

  

   while(check != 3){
    authentication_process();

   } 






   

   
   
   /* Clean up */   
   //free(decrypted_message);
   //free(encrypt_message);
   free(vehicle_pub_key);
   free(vehicle_priv_key);
   free(challenge);
   EVP_cleanup();
   ERR_free_strings();
    return 0;
}



