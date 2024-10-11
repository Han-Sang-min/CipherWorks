#ifndef TESTDLL_LIBRARY_H
#define TESTDLL_LIBRARY_H

#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif // TESTDLL_EXPORTS

#include <openssl/applink.c>

enum SIZE : int {
    KEY_SIZE = 32,
    IV_SIZE = 16
};

struct EncryptedData {
    unsigned char* ciphertext;
    int ciphertext_len;
    unsigned char* iv;
};

struct DecryptedData {
    unsigned char* plaintext;
    int plaintext_len;
};

extern "C" {
    // return ciphertext and iv
    TESTDLL_API EncryptedData* encryptData(unsigned char* plaintext, int plaintext_len, unsigned char* key);
    // return plaintext
    TESTDLL_API DecryptedData* decryptData(EncryptedData* encrypted_data, const unsigned char* key);
    // total len + ciphertext_len + ciphertext + iv_len + iv
    TESTDLL_API unsigned char* serializeEncryptedData(const EncryptedData* data, int *totalLen);
    TESTDLL_API EncryptedData* deserializeEncryptedData(const unsigned char* buffer);

    TESTDLL_API EncryptedData* createEncryptedData(unsigned char* ciphertext, int ciphertext_len, unsigned char* iv);
    TESTDLL_API unsigned char* getciphertext(const EncryptedData &encrypted_data);
    TESTDLL_API int getciphertext_len(const EncryptedData &encrypted_data);
    TESTDLL_API unsigned char* getiv(const EncryptedData &encrypted_data);

    TESTDLL_API unsigned char* getplaintext(const DecryptedData &decrypted_data);
    TESTDLL_API int getplaintext_len(const DecryptedData &decrypted_data);

    // return key
    TESTDLL_API unsigned char* generateKey();
    TESTDLL_API void freeMemory(const unsigned char* data);
    TESTDLL_API void freeEncryptedData(const EncryptedData* encrypted_data);
    TESTDLL_API void freeDecryptedData(const DecryptedData* decrypted_data);
}

void handleErrors();

#endif //TESTDLL_LIBRARY_H
