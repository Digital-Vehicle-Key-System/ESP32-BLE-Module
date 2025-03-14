

#include "crypto.h"



/* Print OpenSSL errors and exit */
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}


/*
 * generate_keys
 *
 * Generates an RSA key pair (public and private keys) and returns them as 
 * PEM-encoded strings stored in dynamically allocated memory.
 *
 * Parameters:
 *   public_key  : Pointer to a char* that will receive the PEM-encoded public key.
 *   private_key : Pointer to a char* that will receive the PEM-encoded private key.
 *
 * Process:
 *   1. Initializes an EVP_PKEY context for RSA key generation.
 *   2. Sets the RSA key size to 2048 bits.
 *   3. Generates the RSA key pair and stores it in an EVP_PKEY structure.
 *   4. Encodes the public key into a PEM-formatted string.
 *   5. Encodes the private key into a PEM-formatted string.
 *   6. Allocates memory for both keys and assigns them to the provided pointers.
 *   7. Cleans up temporary resources to prevent memory leaks.
 *
 * Returns:
 *   The generated keys are stored in the provided pointers. 
 *   The caller is responsible for freeing the allocated memory for `public_key` 
 *   and `private_key` using `free()`.
 *   On error, the function calls handleErrors() and aborts.
 */
void generate_keys(char **public_key, char **private_key) {
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) {
        handleErrors();
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handleErrors();
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handleErrors();
    }

    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handleErrors();
    }

    // Write the public key to a memory BIO.
    BIO *pub_bio = BIO_new(BIO_s_mem());
    if (!pub_bio)
        handleErrors();
    if (PEM_write_bio_PUBKEY(pub_bio, pkey) != 1)
        handleErrors();

    BUF_MEM *pub_buf;
    BIO_get_mem_ptr(pub_bio, &pub_buf);
    // Allocate a string to hold the public key, adding one extra byte for '\0'
    char *pub_str = malloc(pub_buf->length + 1);
    if (!pub_str)
        handleErrors();
    memcpy(pub_str, pub_buf->data, pub_buf->length);
    pub_str[pub_buf->length] = '\0';

    // Write the private key to a memory BIO.
    BIO *priv_bio = BIO_new(BIO_s_mem());
    if (!priv_bio)
        handleErrors();
    if (PEM_write_bio_PrivateKey(priv_bio, pkey, NULL, NULL, 0, NULL, NULL) != 1)
        handleErrors();

    BUF_MEM *priv_buf;
    BIO_get_mem_ptr(priv_bio, &priv_buf);
    // Allocate a string to hold the private key.
    char *priv_str = malloc(priv_buf->length + 1);
    if (!priv_str)
        handleErrors();
    memcpy(priv_str, priv_buf->data, priv_buf->length);
    priv_str[priv_buf->length] = '\0';

    // Set the output pointers.
    *public_key = pub_str;
    *private_key = priv_str;

    // Clean up.
    BIO_free_all(pub_bio);
    BIO_free_all(priv_bio);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
}


/*
 * encrypt_message_to_array_from_memory
 *
 * Encrypts the given plaintext using a public key (in PEM format) provided as a string.
 *
 * Parameters:
 *   plaintext       : A null-terminated string to encrypt.
 *   pubkey_data     : A null-terminated PEM formatted public key string.
 *   ciphertext_len  : Pointer where the length of the ciphertext will be stored.
 *
 * Returns:
 *   A pointer to a dynamically allocated ciphertext buffer. The caller must free it.
 *   On error, the function calls handleErrors() and aborts.
 */
unsigned char *encrypt_message_to_array(const char *plaintext,const char *pubkey_data,size_t *ciphertext_len) {
    // Create a memory BIO from the public key string.
    BIO *bio = BIO_new_mem_buf(pubkey_data, -1); // -1: auto-detect string length
    if (!bio) {
    handleErrors();
    }

    // Read the public key from the BIO.
    EVP_PKEY *pubkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!pubkey) {
    fprintf(stderr, "Error reading public key from memory.\n");
    handleErrors();
    }

    // Create a context for encryption.
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx)
    handleErrors();
    if (EVP_PKEY_encrypt_init(ctx) <= 0)
    handleErrors();

    // Determine buffer length for ciphertext.
    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, NULL, &outlen, (unsigned char *)plaintext, strlen(plaintext) + 1) <= 0)
    handleErrors();

    // Allocate memory for ciphertext.
    unsigned char *outbuf = malloc(outlen);
    if (!outbuf) {
    fprintf(stderr, "Memory allocation error during encryption.\n");
    exit(EXIT_FAILURE);
    }

    // Perform encryption.
    if (EVP_PKEY_encrypt(ctx, outbuf, &outlen, (unsigned char *)plaintext, strlen(plaintext) + 1) <= 0)
    handleErrors();

    *ciphertext_len = outlen;

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pubkey);

    printf("Encryption successful. Ciphertext length: %zu bytes.\n", outlen);

return outbuf;
}




/*
 * decrypt_message_from_memory
 *
 * Parameters:
 *  - private_key_pem: A null-terminated string with the private key in PEM format.
 *  - encrypted: Pointer to the encrypted data (byte array).
 *  - encrypted_len: Length of the encrypted data.
 *
 * Returns:
 *   A pointer to a dynamically allocated null-terminated string containing
 *   the decrypted message. The caller must free the returned string.
 */
char *decrypt_message(const char *private_key_pem,  const unsigned char *encrypted,size_t encrypted_len) {
    // Create a memory BIO for the private key.
    BIO *key_bio = BIO_new_mem_buf(private_key_pem, -1);  /* -1: auto-detect length */
    if (!key_bio)
        handleErrors();
    
    // Read the private key from the BIO.
    EVP_PKEY *privkey = PEM_read_bio_PrivateKey(key_bio, NULL, NULL, NULL);
    BIO_free(key_bio);
    if (!privkey) {
        fprintf(stderr, "Error reading private key from memory\n");
        handleErrors();
    }
    
    // Create a decryption context.
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx)
        handleErrors();
    if (EVP_PKEY_decrypt_init(ctx) <= 0)
        handleErrors();
    
    // Determine the buffer size needed for plaintext.
    size_t outlen;
    if (EVP_PKEY_decrypt(ctx, NULL, &outlen, encrypted, encrypted_len) <= 0)
        handleErrors();
    
    unsigned char *outbuf = malloc(outlen);
    if (!outbuf) {
        fprintf(stderr, "Memory allocation error during decryption\n");
        exit(EXIT_FAILURE);
    }
    
    // Perform decryption.
    if (EVP_PKEY_decrypt(ctx, outbuf, &outlen, encrypted, encrypted_len) <= 0)
        handleErrors();
    
    // Allocate one extra byte for null termination.
    char *decrypted_message = malloc(outlen + 1);
    if (!decrypted_message) {
        fprintf(stderr, "Memory allocation error for final message\n");
        exit(EXIT_FAILURE);
    }
    memcpy(decrypted_message, outbuf, outlen);
    decrypted_message[outlen] = '\0';  // Null-terminate.
    
    // Clean up.
    free(outbuf);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(privkey);
    
    return decrypted_message;
}



/*---------- Helper Functions ----------*/

// Save a byte array to a file in binary mode.
int save_bytes_to_file(const char *filename, const unsigned char *data, size_t data_len) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file for writing");
        return 0;
    }
    size_t written = fwrite(data, 1, data_len, fp);
    fclose(fp);
    return (written == data_len);
}



// Read a fixed number of bytes from a file in binary mode.
int read_bytes_from_file(const char *filename, unsigned char *data, size_t data_len) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file for reading");
        return 0;
    }
    size_t read = fread(data, 1, data_len, fp);
    fclose(fp);
    return (read == data_len);
}

// Utility function to print a buffer as hexadecimal.
void print_hex(const char *label, const unsigned char *data, int len) {
    printf("%s", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}


// Function to generate an AES-256 key and IV, and return them via arrays.
int generate_aes_key(unsigned char *key, unsigned char *iv, int key_len) {
    // Generate random key bytes.
    if (RAND_bytes(key, key_len) != 1) {
        fprintf(stderr, "Error generating AES key bytes.\n");
        return 0;
    }
    // Generate random IV.
    if (RAND_bytes(iv, AES_BLOCK_SIZE) != 1) {
        fprintf(stderr, "Error generating AES IV bytes.\n");
        return 0;
    }
    printf("AES-256 key and IV generated successfully.\n");
    return 1;
}


/*
 * aes_encrypt
 *
 * Encrypts the plaintext using AES-256-CBC with the supplied key and IV.
 *
 * Parameters:
 *   - plaintext: pointer to plaintext bytes.
 *   - plaintext_len: length of plaintext.
 *   - key: pointer to the AES key.
 *   - iv: pointer to the AES IV.
 *   - ciphertext: buffer for the output ciphertext (must be large enough).
 *
 * Returns the length of the ciphertext on success, or -1 on failure.
 */
int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        handleErrors();
    
    int len, ciphertext_len;
    
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
        handleErrors();
    
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
        handleErrors();
    ciphertext_len = len;
    
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
        handleErrors();
    ciphertext_len += len;
    
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

/*
 * aes_decrypt
 *
 * Decrypts the ciphertext using AES-256-CBC with the supplied key and IV.
 *
 * Parameters:
 *   - ciphertext: pointer to ciphertext bytes.
 *   - ciphertext_len: length of the ciphertext.
 *   - key: pointer to the AES key.
 *   - iv: pointer to the AES IV.
 *   - plaintext: buffer for the output plaintext (must be large enough).
 *
 * Returns the length of the plaintext on success, or -1 on failure.
 */
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
    const unsigned char *key, const unsigned char *iv,
    unsigned char *plaintext) {
EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
if (!ctx)
handleErrors();

int len, plaintext_len;

if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
handleErrors();

if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
handleErrors();
plaintext_len = len;

if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
handleErrors();
plaintext_len += len;

EVP_CIPHER_CTX_free(ctx);
return plaintext_len;
}






/*--------------------------------------------------------------
* Generates a random challenge and returns it as a hex string.
* The caller must free the returned string.*/
char *generate_random_challenge(int num_bytes) {
    unsigned char *raw = malloc(num_bytes);
    if (!raw) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    if (RAND_bytes(raw, num_bytes) != 1) {
        fprintf(stderr, "Error generating random bytes\n");
        free(raw);
        exit(EXIT_FAILURE);
    }
    // Allocate a string to hold the hex (2 characters per byte + null terminator)
    char *hex = malloc(num_bytes * 2 + 1);
    if (!hex) {
        fprintf(stderr, "Memory allocation error\n");
        free(raw);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_bytes; i++) {
        sprintf(hex + i*2, "%02x", raw[i]);
    }
    hex[num_bytes * 2] = '\0';
    free(raw);
    return hex;
}

int compare_challenge(const char *original, const char *decrypted) {
    if (strncmp(original, decrypted , AES_KEYLEN) == 0){
        printf("Challenge match: SUCCESS\n");
        return 1;
    }    
    else{
        printf("Challenge match: FAILURE\n");
    }
    return 0;
   
}

