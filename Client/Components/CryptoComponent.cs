using System;
using System.Runtime.InteropServices;

namespace workhardd.Components
{
    [StructLayout(LayoutKind.Sequential)]
    public struct EncryptedData
    {
        public IntPtr ciphertext;
        public int ciphertext_len;
        public IntPtr iv;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct DecryptedData
    {
        public IntPtr plaintext;
        public int plaintext_len;
    }
    
    public class CryptoComponent
    {
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr encryptData(IntPtr plaintext, int plaintextLen, IntPtr key);
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr decryptData(IntPtr encryptedData, IntPtr key);
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr generateKey();
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void freeMemory(IntPtr data);
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr serializeEncryptedData(IntPtr encryptedData, ref int totalLength);
        [DllImport("testDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr deserializeEncryptedData(IntPtr serializedEncryptedData);

        private IntPtr _key = default;

        public void SetKey(IntPtr key)
        {
            if (_key != default) 
                freeMemory(_key);
            _key = key;
        }
        
        public IntPtr GetKey()
        {
            return _key;
        }
        
        public IntPtr GenerateKeyWrapper()
        {
            if (_key != default)
            {
                freeMemory(_key);
            }
            _key = generateKey();
            return _key;
        }
        
        public IntPtr EncryptText(string inputText, ref int totalLength)
        {
            byte[] plaintextBytes = System.Text.Encoding.UTF8.GetBytes(inputText);
            IntPtr plaintext = Marshal.AllocHGlobal(plaintextBytes.Length);
            Marshal.Copy(plaintextBytes, 0, plaintext, plaintextBytes.Length);

            IntPtr encryptedDataPtr = encryptData(plaintext, plaintextBytes.Length, _key);
            IntPtr serializedEncryptedData = serializeEncryptedData(encryptedDataPtr, ref totalLength);
            // EncryptedData encrypted = Marshal.PtrToStructure<EncryptedData>(encryptedDataPtr);
            freeMemory(encryptedDataPtr);
            Marshal.FreeHGlobal(plaintext);

            return serializedEncryptedData;
        }

        public string DecryptText(IntPtr serializedEncryptedData)
        {
            IntPtr encryptedDataPtr = deserializeEncryptedData(serializedEncryptedData);
            IntPtr decryptedDataPtr = decryptData(encryptedDataPtr, _key);
            DecryptedData decrypted = Marshal.PtrToStructure<DecryptedData>(decryptedDataPtr);
            freeMemory(decryptedDataPtr);
            byte[] decryptedBytes = new byte[decrypted.plaintext_len];
            Marshal.Copy(decrypted.plaintext, decryptedBytes, 0, decrypted.plaintext_len);
            string decryptedString = System.Text.Encoding.UTF8.GetString(decryptedBytes);

            freeMemory(decrypted.plaintext);
            return decryptedString;
        }
        
        public void FreeEncryptedData(EncryptedData encryptedData)
        {
            freeMemory(encryptedData.ciphertext);
            freeMemory(encryptedData.iv);
        }
        
        ~CryptoComponent()
        {
            // if (_key != default)
            // {
            //     freeMemory(_key);
            //     _key = default;
            // }
        }
    }
}