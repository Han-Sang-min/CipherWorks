package com.example.demo;

import org.springframework.web.bind.annotation.*;

import java.nio.charset.StandardCharsets;

@RestController
public class HttpsController {
    private final NativeCrypto nativeCrypto = new NativeCrypto();

    private byte[] key = null;

    @PostMapping("/setKey")
    public String setKey(@RequestBody byte[] key) {
        if (key == null || key.length != 32) {
            return "Invalid key";
        }
        this.key = key;
        return "Key set successfully";
    }

    @PostMapping("/process")
    public byte[] processRequest(@RequestBody byte[] requestBody) {
        long serializedDataPtr = nativeCrypto.deserializeEncryptedData(requestBody);
        long decryptedDataPtr = nativeCrypto.decryptData(serializedDataPtr, key);

        nativeCrypto.freeMemory(serializedDataPtr);

        byte[] plaintextByte = nativeCrypto.getPlaintext(decryptedDataPtr);
        String plaintextString = new String(plaintextByte);

        nativeCrypto.freeDecryptedData(decryptedDataPtr);

        String reversedUppercaseString = new StringBuilder(plaintextString.toUpperCase()).reverse().toString();
        byte[] reversedUppercaseBytes = reversedUppercaseString.getBytes(StandardCharsets.UTF_8);

        long EncryptDataPtr = nativeCrypto.encryptData(reversedUppercaseBytes, reversedUppercaseString.length(), key);
        int[] dataLength = new int[1];

        return nativeCrypto.serializeEncryptedData(EncryptDataPtr, dataLength);
    }
}