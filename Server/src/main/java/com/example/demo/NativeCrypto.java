package com.example.demo;

public class NativeCrypto {
    public native long encryptData(byte[] plaintext, int plaintextLen, byte[] key);
    public native long decryptData(long encryptedDataPtr, byte[] key);
    public native long createEncryptedData(byte[] plaintext, int plaintextLen, byte[] key);
    public native byte[] getCiphertext(long encryptedDataPtr);
    public native byte[] getIV(long encryptedDataPtr);
    public native byte[] getPlaintext(long decryptedDataPtr);
    public native void freeMemory(long dataPtr);
    public native void freeMemory(byte[] dataPtr);
    public native void freeEncryptedData(long encryptedDataPtr);
    public native void freeDecryptedData(long decryptedDataPtr);
    public native byte[] serializeEncryptedData(long encryptedDataPtr, int[] totalLength);
    public native long deserializeEncryptedData(byte[] SerializedEncryptedDataPtr);

    static {
        System.loadLibrary("testDLL");
    }
}