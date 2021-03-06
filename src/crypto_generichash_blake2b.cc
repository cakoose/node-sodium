/**
 * Node Native Module for Lib Sodium
 *
 * @Author Pedro Paixao
 * @email paixaop at gmail dot com
 * @License MIT
 */
#include "node_sodium.h"

/**
 * int crypto_generichash_blake2b(unsigned char *out,
 *                        size_t outlen,
 *                        const unsigned char *in,
 *                        unsigned long long inlen,
 *                        const unsigned char *key,
 *                        size_t keylen);
 *  buffer out,
 *  number out_size,
 *  buffer in,
 *  buffer key
 */
NAPI_METHOD(crypto_generichash_blake2b) {
    Napi::Env env = info.Env();

    ARGS(3,"arguments must be: hash size, message, key");
    ARG_TO_NUMBER(out_size);
    ARG_TO_UCHAR_BUFFER(in);
    ARG_TO_UCHAR_BUFFER(key);

    CHECK_SIZE(key_size, crypto_generichash_blake2b_KEYBYTES_MIN, crypto_generichash_blake2b_KEYBYTES_MAX);
    CHECK_SIZE(out_size, crypto_generichash_blake2b_BYTES_MIN, crypto_generichash_blake2b_BYTES_MAX);

    NEW_BUFFER_AND_PTR(hash, out_size);
    sodium_memzero(hash_ptr, out_size);

    if (crypto_generichash_blake2b(hash_ptr, out_size, in, in_size, key, key_size) == 0) {
        return hash;
    }

    return env.Null();
}

/*
int crypto_generichash_blake2b_init(crypto_generichash_blake2b_state *state,
                            const unsigned char *key,
                            const size_t keylen, const size_t outlen);
  Buffer state
  Buffer key
  Number out_size
  state = sodium_malloc((crypto_generichash_blake2b_statebytes() + (size_t) 63U)
 *                       & ~(size_t) 63U);
*/
NAPI_METHOD(crypto_generichash_blake2b_init) {
    Napi::Env env = info.Env();

    ARGS(2,"arguments must be: key buffer, output size");
    ARG_TO_UCHAR_BUFFER(key);
    ARG_TO_NUMBER(out_size);

    CHECK_SIZE(key_size, crypto_generichash_blake2b_KEYBYTES_MIN, crypto_generichash_blake2b_KEYBYTES_MAX);
    CHECK_SIZE(out_size, crypto_generichash_blake2b_BYTES_MIN, crypto_generichash_blake2b_BYTES_MAX);

    NEW_BUFFER_AND_PTR(state, (crypto_generichash_blake2b_statebytes() + (size_t) 63U) & ~(size_t) 63U);

    if (crypto_generichash_blake2b_init((crypto_generichash_blake2b_state *)state_ptr, key, key_size, out_size) == 0) {
        return state;
    }

    return env.Null();
}


/*
int crypto_generichash_blake2b_update(crypto_generichash_blake2b_state *state,
                              const unsigned char *in,
                              unsigned long long inlen);

    buffer state
    buffer message
*/
NAPI_METHOD(crypto_generichash_blake2b_update) {
    Napi::Env env = info.Env();

    ARGS(2,"arguments must be: state buffer, message buffer");

    ARG_TO_UCHAR_BUFFER(state); // VOID
    ARG_TO_UCHAR_BUFFER(message);

    NEW_BUFFER_AND_PTR(state2, (crypto_generichash_blake2b_statebytes() + (size_t) 63U) & ~(size_t) 63U);
    memcpy(state2_ptr, state, (crypto_generichash_blake2b_statebytes() + (size_t) 63U) & ~(size_t) 63U);

    crypto_generichash_blake2b_update((crypto_generichash_blake2b_state *)state2_ptr, message, message_size);
    return state2;
}

/*
int crypto_generichash_blake2b_final(crypto_generichash_blake2b_state *state,
                             unsigned char *out, const size_t outlen);
*/
NAPI_METHOD(crypto_generichash_blake2b_final) {
    Napi::Env env = info.Env();

    ARGS(2,"arguments must be: state buffer, output size");
    ARG_TO_UCHAR_BUFFER(state);
    ARG_TO_NUMBER(out_size);

    CHECK_SIZE(out_size, crypto_generichash_blake2b_BYTES_MIN, crypto_generichash_blake2b_BYTES_MAX);

    NEW_BUFFER_AND_PTR(hash, out_size);

    if (crypto_generichash_blake2b_final((crypto_generichash_blake2b_state *)state, hash_ptr, out_size) == 0) {
        return hash;
    }

    return env.Null();
}

/*
 *int crypto_generichash_blake2b_salt_personal(unsigned char *out, size_t outlen,
                                             const unsigned char *in,
                                             unsigned long long inlen,
                                             const unsigned char *key,
                                             size_t keylen,
                                             const unsigned char *salt,
                                             const unsigned char *personal);

    Buffer out
    Buffer in
    Buffer key
    Buffer salt
    Buffer personal
 */
NAPI_METHOD(crypto_generichash_blake2b_salt_personal) {
    Napi::Env env = info.Env();

    ARGS(5,"arguments must five buffers: output, message, key, salt, personal");
    ARG_TO_UCHAR_BUFFER(out);
    ARG_TO_UCHAR_BUFFER(in);
    ARG_TO_UCHAR_BUFFER(key);
    ARG_TO_UCHAR_BUFFER_LEN(salt, crypto_generichash_blake2b_SALTBYTES);
    ARG_TO_UCHAR_BUFFER_LEN(personal, crypto_generichash_blake2b_PERSONALBYTES);

    CHECK_SIZE(out_size, crypto_generichash_blake2b_BYTES_MIN, crypto_generichash_blake2b_BYTES_MAX);
    CHECK_SIZE(key_size, crypto_generichash_blake2b_KEYBYTES_MIN, crypto_generichash_blake2b_KEYBYTES_MAX);

    sodium_memzero(out, out_size);
    if (crypto_generichash_blake2b_salt_personal(out, out_size, in, in_size, key, key_size, salt, personal) == 0) {
        return Napi::Boolean::New(env, true);
    }

    return Napi::Boolean::New(env, false);
}

/**
 * Register function calls in node binding
 */
void register_crypto_generichash_blake2b(Napi::Env env, Napi::Object exports) {

     // Generic Hash
    EXPORT(crypto_generichash_blake2b);
    EXPORT(crypto_generichash_blake2b_init);
    EXPORT(crypto_generichash_blake2b_update);
    EXPORT(crypto_generichash_blake2b_final);
    EXPORT(crypto_generichash_blake2b_salt_personal);

    EXPORT_INT(crypto_generichash_blake2b_BYTES);
    EXPORT_INT(crypto_generichash_blake2b_BYTES_MIN);
    EXPORT_INT(crypto_generichash_blake2b_BYTES_MAX);
    EXPORT_INT(crypto_generichash_blake2b_KEYBYTES);
    EXPORT_INT(crypto_generichash_blake2b_KEYBYTES_MIN);
    EXPORT_INT(crypto_generichash_blake2b_KEYBYTES_MAX);
    EXPORT_INT(crypto_generichash_blake2b_SALTBYTES);
    EXPORT_INT(crypto_generichash_blake2b_PERSONALBYTES);
}