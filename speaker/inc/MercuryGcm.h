#ifndef _MERCURY_GCM_H_
#define _MERCURY_GCM_H_



#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif



#define GCM_ENCRYPT     1
#define GCM_DECRYPT     0


typedef void* GCM_CTX;


typedef enum {
    CIPHER_ID_NONE = 0,  /**< Placeholder to mark the end of cipher ID lists. */
    CIPHER_ID_NULL,      /**< The identity cipher, treated as a stream cipher. */
    CIPHER_ID_AES,       /**< The AES cipher. */
    CIPHER_ID_DES,       /**< The DES cipher. */
    CIPHER_ID_3DES,      /**< The Triple DES cipher. */
    CIPHER_ID_CAMELLIA,  /**< The Camellia cipher. */
    CIPHER_ID_BLOWFISH,  /**< The Blowfish cipher. */
    CIPHER_ID_ARC4,      /**< The RC4 cipher. */
    CIPHER_ID_ARIA,      /**< The Aria cipher. */
    CIPHER_ID_CHACHA20,  /**< The ChaCha20 cipher. */
} CHIPER_ID_E;


/**
 * \brief           This function initializes the specified GCM context,
 *                  to make references valid, and prepares the context
 *                  for Gcm_setkey() or Gcm_free().
 *
 *                  The function does not bind the GCM context to a particular
 *                  cipher, nor set the key. For this purpose, use
 *                  Gcm_setkey().
 *
 * \param ctx       The GCM context to initialize.
 */
int Gcm_init(GCM_CTX *ctx);

/**
 * \brief           This function associates a GCM context with a
 *                  cipher algorithm and a key.
 *
 * \param ctx       The GCM context to initialize.
 * \param cipher    The 128-bit block cipher to use.
 * \param key       The encryption key.
 * \param keybits   The key size in bits. Valid options are:
 *                  <ul><li>128 bits</li>
 *                  <li>192 bits</li>
 *                  <li>256 bits</li></ul>
 *
 * \return          \c 0 on success.
 * \return          A cipher-specific error code on failure.
 */
int Gcm_setkey(GCM_CTX ctx,
                        CHIPER_ID_E cipher,
                        const unsigned char *key,
                        unsigned int keybits);

/**
 * \brief           This function performs GCM encryption or decryption of a buffer.
 *
 * \note            For encryption, the output buffer can be the same as the
 *                  input buffer. For decryption, the output buffer cannot be
 *                  the same as input buffer. If the buffers overlap, the output
 *                  buffer must trail at least 8 Bytes behind the input buffer.
 *
 * \warning         When this function performs a decryption, it outputs the
 *                  authentication tag and does not verify that the data is
 *                  authentic. You should use this function to perform encryption
 *                  only. For decryption, use Gcm_auth_decrypt() instead.
 *
 * \param ctx       The GCM context to use for encryption or decryption.
 * \param mode      The operation to perform:
 *                  - #MBEDTLS_GCM_ENCRYPT to perform authenticated encryption.
 *                    The ciphertext is written to \p output and the
 *                    authentication tag is written to \p tag.
 *                  - #MBEDTLS_GCM_DECRYPT to perform decryption.
 *                    The plaintext is written to \p output and the
 *                    authentication tag is written to \p tag.
 *                    Note that this mode is not recommended, because it does
 *                    not verify the authenticity of the data. For this reason,
 *                    you should use Gcm_auth_decrypt() instead of
 *                    calling this function in decryption mode.
 * \param length    The length of the input data, which is equal to the length
 *                  of the output data.
 * \param iv        The initialization vector.
 * \param iv_len    The length of the IV.
 * \param add       The buffer holding the additional data.
 * \param add_len   The length of the additional data.
 * \param input     The buffer holding the input data. Its size is \b length.
 * \param output    The buffer for holding the output data. It must have room
 *                  for \b length bytes.
 * \param tag_len   The length of the tag to generate.
 * \param tag       The buffer for holding the tag.
 *
 * \return          \c 0 if the encryption or decryption was performed
 *                  successfully. Note that in #MBEDTLS_GCM_DECRYPT mode,
 *                  this does not indicate that the data is authentic.
 * \return          #MBEDTLS_ERR_GCM_BAD_INPUT if the lengths are not valid or
 *                  a cipher-specific error code if the encryption
 *                  or decryption failed.
 */
int Gcm_crypt_and_tag(GCM_CTX ctx,
                       int mode,
                       size_t length,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t tag_len,
                       unsigned char *tag);

/**
 * \brief           This function performs a GCM authenticated decryption of a
 *                  buffer.
 *
 * \note            For decryption, the output buffer cannot be the same as
 *                  input buffer. If the buffers overlap, the output buffer
 *                  must trail at least 8 Bytes behind the input buffer.
 *
 * \param ctx       The GCM context.
 * \param length    The length of the ciphertext to decrypt, which is also
 *                  the length of the decrypted plaintext.
 * \param iv        The initialization vector.
 * \param iv_len    The length of the IV.
 * \param add       The buffer holding the additional data.
 * \param add_len   The length of the additional data.
 * \param tag       The buffer holding the tag to verify.
 * \param tag_len   The length of the tag to verify.
 * \param input     The buffer holding the ciphertext. Its size is \b length.
 * \param output    The buffer for holding the decrypted plaintext. It must
 *                  have room for \b length bytes.
 *
 * \return          \c 0 if successful and authenticated.
 * \return          #MBEDTLS_ERR_GCM_AUTH_FAILED if the tag does not match.
 * \return          #MBEDTLS_ERR_GCM_BAD_INPUT if the lengths are not valid or
 *                  a cipher-specific error code if the decryption failed.
 */
int Gcm_auth_decrypt(GCM_CTX ctx,
                      size_t length,
                      const unsigned char *iv,
                      size_t iv_len,
                      const unsigned char *add,
                      size_t add_len,
                      const unsigned char *tag,
                      size_t tag_len,
                      const unsigned char *input,
                      unsigned char *output);

/**
 * \brief           This function starts a GCM encryption or decryption
 *                  operation.
 *
 * \param ctx       The GCM context.
 * \param mode      The operation to perform: #MBEDTLS_GCM_ENCRYPT or
 *                  #MBEDTLS_GCM_DECRYPT.
 * \param iv        The initialization vector.
 * \param iv_len    The length of the IV.
 * \param add       The buffer holding the additional data, or NULL
 *                  if \p add_len is 0.
 * \param add_len   The length of the additional data. If 0,
 *                  \p add is NULL.
 *
 * \return          \c 0 on success.
 */
int Gcm_starts(GCM_CTX ctx,
                int mode,
                const unsigned char *iv,
                size_t iv_len,
                const unsigned char *add,
                size_t add_len);

/**
 * \brief           This function feeds an input buffer into an ongoing GCM
 *                  encryption or decryption operation.
 *
 *    `             The function expects input to be a multiple of 16
 *                  Bytes. Only the last call before calling
 *                  Gcm_finish() can be less than 16 Bytes.
 *
 * \note            For decryption, the output buffer cannot be the same as
 *                  input buffer. If the buffers overlap, the output buffer
 *                  must trail at least 8 Bytes behind the input buffer.
 *
 * \param ctx       The GCM context.
 * \param length    The length of the input data. This must be a multiple of
 *                  16 except in the last call before Gcm_finish().
 * \param input     The buffer holding the input data.
 * \param output    The buffer for holding the output data.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_GCM_BAD_INPUT on failure.
 */
int Gcm_update(GCM_CTX ctx,
                size_t length,
                const unsigned char *input,
                unsigned char *output);

/**
 * \brief           This function finishes the GCM operation and generates
 *                  the authentication tag.
 *
 *                  It wraps up the GCM stream, and generates the
 *                  tag. The tag can have a maximum length of 16 Bytes.
 *
 * \param ctx       The GCM context.
 * \param tag       The buffer for holding the tag.
 * \param tag_len   The length of the tag to generate. Must be at least four.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_GCM_BAD_INPUT on failure.
 */
int Gcm_finish(GCM_CTX ctx,
                unsigned char *tag,
                size_t tag_len);

/**
 * \brief           This function clears a GCM context and the underlying
 *                  cipher sub-context.
 *
 * \param ctx       The GCM context to clear.
 */
int Gcm_free(GCM_CTX ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_GCM_H_

