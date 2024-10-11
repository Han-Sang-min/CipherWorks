#include "library.h"

#include <cstring>

#include "com_example_demo_NativeCrypto.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

unsigned char* generateKey() {
    auto* key = new unsigned char[KEY_SIZE];
    RAND_bytes(key, KEY_SIZE);
    return key;
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

EncryptedData* encryptData(unsigned char* plaintext, int plaintext_len, unsigned char* key) {
    auto* iv = new unsigned char[IV_SIZE];
    RAND_bytes(iv, IV_SIZE);

    auto* ciphertext = new unsigned char[plaintext_len + EVP_CIPHER_block_size(EVP_aes_256_cbc())];
    int len = 0, ciphertext_len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    auto* encrypted_data = new EncryptedData();
    encrypted_data->ciphertext = ciphertext;
    encrypted_data->ciphertext_len = ciphertext_len;
    encrypted_data->iv = iv;
    return encrypted_data;
}

DecryptedData* decryptData(EncryptedData* encrypted_data, const unsigned char* key) {
    unsigned char* iv = encrypted_data->iv;
    unsigned char* ciphertext = encrypted_data->ciphertext;
    int ciphertext_len = encrypted_data->ciphertext_len;
    auto* plaintext = new unsigned char[ciphertext_len + EVP_MAX_BLOCK_LENGTH];

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) handleErrors();

    int len = 0;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    int plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    auto decrypted_data = new DecryptedData();
    decrypted_data->plaintext = plaintext;
    decrypted_data->plaintext_len = plaintext_len;
    return decrypted_data;
}

void freeMemory(const unsigned char *data) {
    delete[] data;
}

unsigned char * getciphertext(const EncryptedData &encrypted_data) {
    return encrypted_data.ciphertext;
}


int getciphertext_len(const EncryptedData &encrypted_data) {
    return encrypted_data.ciphertext_len;
}


unsigned char * getiv(const EncryptedData &encrypted_data) {
    return encrypted_data.iv;
}


unsigned char * getplaintext(const DecryptedData &decrypted_data) {
    return decrypted_data.plaintext;
}

int getplaintext_len(const DecryptedData &decrypted_data) {
    return decrypted_data.plaintext_len;
}

EncryptedData * createEncryptedData(unsigned char *ciphertext, int ciphertext_len, unsigned char *iv) {
    auto* encrypted_data = new EncryptedData();
    encrypted_data->ciphertext = ciphertext;
    encrypted_data->ciphertext_len = ciphertext_len;
    encrypted_data->iv = iv;
    return encrypted_data;
}

void freeEncryptedData(const EncryptedData *encrypted_data) {
    delete[] encrypted_data->ciphertext;
    delete[] encrypted_data->iv;
    delete encrypted_data;
}

void freeDecryptedData(const DecryptedData *decrypted_data) {
    delete[] decrypted_data->plaintext;
    delete decrypted_data;
}

JNIEXPORT jlong JNICALL Java_com_example_demo_NativeCrypto_decryptData
  (JNIEnv *env, jobject obj, jlong encryptedDataPtr, jbyteArray key) {
    auto* encryptedData = reinterpret_cast<EncryptedData*>(encryptedDataPtr);
    jbyte* keyBytes = env->GetByteArrayElements(key, nullptr);

    DecryptedData* decryptedData = decryptData(encryptedData, (unsigned char*)keyBytes);

    env->ReleaseByteArrayElements(key, keyBytes, 0);

    return reinterpret_cast<jlong>(decryptedData);
}

// JNIEXPORT jlong JNICALL Java_com_example_demo_NativeCrypto_createEncryptedData
//   (JNIEnv *env, jobject obj, jbyteArray ciphertext, jint ciphertext_len, jbyteArray iv) {
//     jbyte* ciphertextBytes = env->GetByteArrayElements(ciphertext, nullptr);
//     jbyte* ivBytes = env->GetByteArrayElements(iv, nullptr);
//
//     EncryptedData* encryptedData = createEncryptedData((unsigned char*)ciphertextBytes, ciphertext_len, (unsigned char*)ivBytes);
//
//     env->ReleaseByteArrayElements(ciphertext, ciphertextBytes, 0);
//     env->ReleaseByteArrayElements(iv, ivBytes, 0);
//
//     return reinterpret_cast<jlong>(encryptedData);
// }

JNIEXPORT jlong JNICALL Java_com_example_demo_NativeCrypto_encryptData(JNIEnv *env, jobject obj, jbyteArray plaintext, jint plaintextLen, jbyteArray key) {
    jbyte* plaintextBytes = env->GetByteArrayElements(plaintext, nullptr);
    jbyte* keyBytes = env->GetByteArrayElements(key, nullptr);

    EncryptedData* encryptedData = encryptData((unsigned char*)plaintextBytes, plaintextLen, (unsigned char*)keyBytes);

    env->ReleaseByteArrayElements(plaintext, plaintextBytes, 0);
    env->ReleaseByteArrayElements(key, keyBytes, 0);

    return reinterpret_cast<jlong>(encryptedData);
}

JNIEXPORT jbyteArray JNICALL Java_com_example_demo_NativeCrypto_getPlaintext(JNIEnv *env, jobject obj, jlong decryptedDataPtr) {
    DecryptedData* decryptedData = reinterpret_cast<DecryptedData*>(decryptedDataPtr);

    jbyteArray plaintext = env->NewByteArray(decryptedData->plaintext_len);
    env->SetByteArrayRegion(plaintext, 0, decryptedData->plaintext_len, (jbyte*)decryptedData->plaintext);

    return plaintext;
}

JNIEXPORT void JNICALL Java_com_example_demo_NativeCrypto_freeMemory__J
  (JNIEnv *env, jobject obj, jlong dataPtr) {
    auto* data = reinterpret_cast<unsigned char*>(dataPtr);
    freeMemory(data);
}

JNIEXPORT void JNICALL Java_com_example_demo_NativeCrypto_freeMemory___3B
  (JNIEnv *env, jobject obj, jbyteArray data) {
    jbyte* dataBytes = env->GetByteArrayElements(data, nullptr);
    freeMemory((unsigned char*)dataBytes);
    env->ReleaseByteArrayElements(data, dataBytes, 0);
}

JNIEXPORT void JNICALL Java_com_example_demo_NativeCrypto_freeEncryptedData(JNIEnv *env, jobject obj, jlong encryptedDataPtr) {
    auto* encryptedData = reinterpret_cast<EncryptedData*>(encryptedDataPtr);
    freeEncryptedData(encryptedData);
}

JNIEXPORT void JNICALL Java_com_example_demo_NativeCrypto_freeDecryptedData(JNIEnv *env, jobject obj, jlong decryptedDataPtr) {
    auto* decryptedData = reinterpret_cast<DecryptedData*>(decryptedDataPtr);
    freeDecryptedData(decryptedData);
}

JNIEXPORT jbyteArray JNICALL Java_com_example_demo_NativeCrypto_serializeEncryptedData
  (JNIEnv *env, jobject obj, jlong encryptedDataPtr, jintArray totalLen) {
    int len = 0;
    auto* encryptedData = reinterpret_cast<EncryptedData*>(encryptedDataPtr);
    unsigned char* buffer = serializeEncryptedData(encryptedData, &len);
    // freeEncryptedData(encryptedData);

    jint* totalLenPtr = env->GetIntArrayElements(totalLen, nullptr);
    totalLenPtr[0] = len;
    env->ReleaseIntArrayElements(totalLen, totalLenPtr, 0);

    jbyteArray byteArray = env->NewByteArray(len);
    if (byteArray == nullptr) {
        delete[] buffer;
        return nullptr;
    }

    env->SetByteArrayRegion(byteArray, 0, len, reinterpret_cast<jbyte*>(buffer));

    delete[] buffer;

    return byteArray;
}

JNIEXPORT jlong JNICALL Java_com_example_demo_NativeCrypto_deserializeEncryptedData
  (JNIEnv *env, jobject obj, jbyteArray buffer) {
    jbyte* bufferBytes = env->GetByteArrayElements(buffer, nullptr);

    EncryptedData* encryptedData = deserializeEncryptedData((unsigned char*)bufferBytes);

    env->ReleaseByteArrayElements(buffer, bufferBytes, 0);

    return reinterpret_cast<jlong>(encryptedData);
}

unsigned char * serializeEncryptedData(const EncryptedData *data, int *totalLen) {
    size_t len = sizeof(int) + data->ciphertext_len + IV_SIZE;
    auto buffer = new unsigned char[len];
    int offset = 0;

    memcpy(buffer + offset, &data->ciphertext_len, sizeof(int));
    offset += sizeof(int);

    memcpy(buffer + offset, data->ciphertext, data->ciphertext_len);
    offset += data->ciphertext_len;

    memcpy(buffer + offset, data->iv, IV_SIZE);
    offset += IV_SIZE;

    *totalLen = static_cast<int>(len);
    return buffer;
}

EncryptedData * deserializeEncryptedData(const unsigned char *buffer) {
    int offset = 0;
    auto* data = new EncryptedData;

    memcpy(&data->ciphertext_len, buffer + offset, sizeof(int));
    offset += sizeof(int);

    data->ciphertext = new unsigned char[data->ciphertext_len];
    memcpy(data->ciphertext, buffer + offset, data->ciphertext_len);
    offset += data->ciphertext_len;

    data->iv = new unsigned char[IV_SIZE];
    memcpy(data->iv, buffer + offset, IV_SIZE);
    offset += IV_SIZE;

    return data;
}
