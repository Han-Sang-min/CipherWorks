using System;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;
using workhardd.Components;

namespace workhardd
{
    public partial class Form1 : Form
    {
        private CryptoComponent _cryptoComponent = new CryptoComponent();
        
        private static readonly HttpClient Client;
        
        static Form1()
        {
            HttpClientHandler handler = new HttpClientHandler();
            handler.ServerCertificateCustomValidationCallback = (message, cert, chain, sslPolicyErrors) => true;
            Client = new HttpClient(handler);
        }
        public Form1()
        {
            InitializeComponent();
            _cryptoComponent.GenerateKeyWrapper();
            _ = SendKeyToServer();
        }

        // private string ConvertEncryptedDataToString(EncryptedData encryptedData)
        // {
        //     byte[] ciphertextBytes = new byte[encryptedData.ciphertext_len];
        //     int ciphertextLength = ciphertextBytes.Length;
        //     Marshal.Copy(encryptedData.ciphertext, ciphertextBytes, 0, encryptedData.ciphertext_len);
        //
        //     byte[] ivBytes = new byte[16];
        //     Marshal.Copy(encryptedData.iv, ivBytes, 0, 16);
        //
        //     string ciphertextBase64 = Convert.ToBase64String(ciphertextBytes);
        //     string ivBase64 = Convert.ToBase64String(ivBytes);
        //
        //     _cryptoComponent.FreeEncryptedData(encryptedData);
        //     return $"ciphertext={ciphertextBase64}&ciphertext_len={ciphertextLength}&iv={ivBase64}";
        // }
        
        private void sendButton_Click(object sender, EventArgs e)
        {
            string inputText = InputBox.Text;
            InputBox.Text = string.Empty;
            if (string.IsNullOrEmpty(inputText))
            {
                return;
            }
            int totalLength = 0;
            IntPtr serializedEncryptedData = _cryptoComponent.EncryptText(inputText, ref totalLength);
            Task.Run(() => SendDataToServer(serializedEncryptedData, totalLength));
        }

        private async Task SendDataToServer(IntPtr serializedEncryptedData, int dataLength)
        {
            string serverUrl = "https://localhost:8443/process";
            try
            {
                byte[] buffer = new byte[dataLength];
                Marshal.Copy(serializedEncryptedData, buffer, 0, dataLength);
                CryptoComponent.freeMemory(serializedEncryptedData);

                ByteArrayContent content = new ByteArrayContent(buffer);
                content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("application/octet-stream");

                HttpResponseMessage response = await Client.PostAsync(serverUrl, content);
                response.EnsureSuccessStatusCode();

                byte[] responseBytes = await response.Content.ReadAsByteArrayAsync();
                ProcessServerResponse(responseBytes);
            }
            catch (Exception ex)
            {
                MessageBox.Show($@"Error occurred: {ex.Message}");
            }
        }
        
        private async Task SendKeyToServer()
        {
            byte[] keyBytes = new byte[32];
            Marshal.Copy(_cryptoComponent.GetKey(), keyBytes, 0, 32);
            
            string serverUrl = "https://localhost:8443/setKey";
            try
            {
                var content = new ByteArrayContent(keyBytes);
                
                HttpResponseMessage response = await Client.PostAsync(serverUrl, content);
                response.EnsureSuccessStatusCode();
                string responseBody = await response.Content.ReadAsStringAsync();
                Console.WriteLine(responseBody);
            }
            catch (Exception ex)
            {
                MessageBox.Show($@"Error occurred: {ex.Message}");
            }
        }

        private void ProcessServerResponse(byte[] responseBytes)
        {
            if (InvokeRequired)
            {
                Invoke(new Action(() => ProcessServerResponse(responseBytes)));
            }
            else
            {
                IntPtr ptr = Marshal.AllocHGlobal(responseBytes.Length);
                Marshal.Copy(responseBytes, 0, ptr, responseBytes.Length);
                String plainText = _cryptoComponent.DecryptText(ptr);
                
                label1.Text = plainText;
            }
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                sendButton_Click(sender, EventArgs.Empty);
                e.SuppressKeyPress = true;
            }
        }
    }
}