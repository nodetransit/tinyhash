/*********************************************************************
* Filename:   aes.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding AES implementation.
*********************************************************************/

#ifndef TINYHASH_AES_H__
#define TINYHASH_AES_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AES Key setup
 * @details Key setup must be done before any AES en/de-cryption functions can be used.
 *
 * @param key     128, 192, or 256 bits
 * @param w       output key
 * @param keysize bit length of the key
 */
void aes_key_setup(const unsigned char key[], unsigned int w[], int keysize);

/**
 * @brief
 *
 * @param in 16 bytes of plaintext
 * @param out 16 bytes of ciphertext
 * @param key From the key setup
 * @param keysize Bit length of the key, 128, 192, or 256
 */
void aes_encrypt(const unsigned char in[], unsigned char out[], const unsigned int key[], int keysize);

/**
 * @brief
 *
 * @param in       16 bytes of ciphertext
 * @param out      16 bytes of plaintext
 * @param key      From the key setup
 * @param keysize  Bit length of the key, 128, 192, or 256
 */
void aes_decrypt(const unsigned char in[], unsigned char out[], const unsigned int key[], int keysize);

/////////////////////////////////////////////////////////////////////.
// AES - CBC
/////////////////////////////////////////////////////////////////////.

/**
 * @brief
 *
 * @param in      Plaintext
 * @param in_len  Must be a multiple of AES_BLOCK_SIZE
 * @param out     Ciphertext, same length as plaintext
 * @param key     From the key setup
 * @param keysize Bit length of the key, 128, 192, or 256
 * @param iv      IV, must be AES_BLOCK_SIZE bytes long
 *
 * @return
 */
int aes_encrypt_cbc(const unsigned char in[], size_t in_len, unsigned char out[], const unsigned int key[], int keysize, const unsigned char iv[]);

int aes_decrypt_cbc(const unsigned char in[], size_t in_len, unsigned char out[], const unsigned int key[], int keysize, const unsigned char iv[]);

/**
 * @brief Only output the CBC-MAC of the input.
 *
 * @param in      plaintext
 * @param in_len  Must be a multiple of AES_BLOCK_SIZE
 * @param out     Output MAC
 * @param key     From the key setup
 * @param keysize Bit length of the key, 128, 192, or 256
 * @param iv      IV, must be AES_BLOCK_SIZE bytes long
 *
 * @return
 */
int
aes_encrypt_cbc_mac(const unsigned char in[], size_t in_len, unsigned char out[], const unsigned int key[], int keysize, const unsigned char iv[]);

/////////////////////////////////////////////////////////////////////.
// AES - CTR
/////////////////////////////////////////////////////////////////////.

/**
 * @brief
 *
 * @param iv           Must be a multiple of AES_BLOCK_SIZE
 * @param counter_size Bytes of the IV used for counting (low end)
 */
void increment_iv(unsigned char iv[], unsigned int counter_size);

/**
 * @brief
 *
 * @param in      Plaintext
 * @param in_len  Any byte length
 * @param out     Ciphertext, same length as plaintext
 * @param key     From the key setup
 * @param keysize Bit length of the key, 128, 192, or 256
 * @param iv      IV, must be AES_BLOCK_SIZE bytes long
 */
void aes_encrypt_ctr(const unsigned char in[], size_t in_len, unsigned char out[], const unsigned int key[], int keysize, const unsigned char iv[]);

/**
 * @brief
 *
 * @param in      Ciphertext
 * @param in_len  Any byte length
 * @param out     Plaintext, same length as ciphertext
 * @param key     From the key setup
 * @param keysize Bit length of the key, 128, 192, or 256
 * @param iv      IV, must be AES_BLOCK_SIZE bytes long
 */
void aes_decrypt_ctr(const unsigned char in[], size_t in_len, unsigned char out[], const unsigned int key[], int keysize, const unsigned char iv[]);

/////////////////////////////////////////////////////////////////////.
// AES - CCM
/////////////////////////////////////////////////////////////////////.

/**
 * @brief Returns True if the input parameters do not violate any constraint.
 *
 * @param plaintext           IN  - Plaintext.
 * @param plaintext_len       IN  - Plaintext length.
 * @param associated_data     IN  - Associated Data included in authentication, but not encryption.
 * @param associated_data_len IN  - Associated Data length in bytes.
 * @param nonce               IN  - The Nonce to be used for encryption.
 * @param nonce_len           IN  - Nonce length in bytes.
 * @param ciphertext          OUT - Ciphertext, a concatination of the plaintext and the MAC.
 * @param ciphertext_len      OUT - The length of the ciphertext, always plaintext_len + mac_len.
 * @param mac_len             IN  - The desired length of the MAC, must be 4, 6, 8, 10, 12, 14, or 16.
 * @param key                 IN  - The AES key for encryption.
 * @param keysize             IN  - The length of the key in bits. Valid values are 128, 192, 256.
 *
 * @return
 */
int aes_encrypt_ccm(const unsigned char plaintext[], unsigned int plaintext_len, const unsigned char associated_data[], unsigned short associated_data_len, const unsigned char nonce[], unsigned short nonce_len, unsigned char ciphertext[], unsigned int* ciphertext_len, unsigned int mac_len, const unsigned char key[], int keysize);

/**
 * @brief Returns True if the input parameters do not violate any constraint.
 * @details
 * Use mac_auth to ensure decryption/validation was preformed correctly.
 * If authentication does not succeed, the plaintext is zeroed out. To overwride
 * this, call with mac_auth = NULL. The proper proceedure is to decrypt with
 * authentication enabled (mac_auth != NULL) and make a second call to that
 * ignores authentication explicitly if the first call failes.
 *
 * @param ciphertext     IN  - Ciphertext, the concatination of encrypted plaintext and MAC.
 * @param ciphertext_len IN  - Ciphertext length in bytes.
 * @param assoc          IN  - The Associated Data, required for authentication.
 * @param assoc_len      IN  - Associated Data length in bytes.
 * @param nonce          IN  - The Nonce to use for decryption, same one as for encryption.
 * @param nonce_len      IN  - Nonce length in bytes.
 * @param plaintext      OUT - The plaintext that was decrypted. Will need to be large enough to hold ciphertext_len - mac_len.
 * @param plaintext_len  OUT - Length in bytes of the output plaintext, always ciphertext_len - mac_len .
 * @param mac_len        IN  - The length of the MAC that was calculated.
 * @param mac_auth       OUT - TRUE if authentication succeeded, FALSE if it did not. NULL pointer will ignore the authentication.
 * @param key            IN  - The AES key for decryption.
 * @param keysize        IN  - The length of the key in BITS. Valid values are 128, 192, 256.
 *
 * @return
 */
int aes_decrypt_ccm(const unsigned char ciphertext[], unsigned int ciphertext_len, const unsigned char assoc[], unsigned short assoc_len, const unsigned char nonce[], unsigned short nonce_len, unsigned char plaintext[], unsigned int* plaintext_len, unsigned int mac_len, int* mac_auth, const unsigned char key[], int keysize);

#ifdef __cplusplus
} //!extern "C"
#endif

#endif /* TINYHASH_AES_H__ */
