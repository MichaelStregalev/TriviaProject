using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace BackendTrivia
{
    internal class Communicator
    {
        //Private variables
        private Socket mClient = null;
        private int mPort = 8876;
        private string mIP = "127.0.0.1";

        public Communicator()
        {
            Console.WriteLine("(っ◕‿◕)っ Starting Trivia client...");

            try
            {
                // Create a TCP/IP socket
                mClient = new Socket(AddressFamily.InterNetwork,
                                            SocketType.Stream,
                                            ProtocolType.Tcp);

                // Server address and port
                IPAddress ipAddress = IPAddress.Parse(mIP); // Change to server IP
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, mPort); // Change port as needed

                // Connect to the server
                Console.WriteLine("(ﾉ◕ヮ◕)ﾉ*:･ﾟ✧ Connecting to Trivia server...");
                mClient.Connect(remoteEP);

                Console.WriteLine("Socket connected to {0} ✧ﾟ･: *ヽ(◕ヮ◕ヽ)",
                                    mClient.RemoteEndPoint.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine("(╥﹏╥) Oh no! Exception: {0}", e.ToString());
            }
        }
        //Functions
        public void Send(int code, string str)
        {
            Info info = new Info(code, str);
            int bytesSent = mClient.Send(info.ToByte());
        }
        public Info Recv()
        {
            // Receive response
            byte[] bytes = new byte[1];
            int bytesRec = mClient.Receive(bytes);
            int code = (int)bytes[0];
            if (bytesRec != 1)
                throw new IOException("Failed to read message type byte");

            bytes = new byte[4];
            bytesRec = mClient.Receive(bytes, 0, 4, SocketFlags.None);
            if (bytesRec != 4)
                throw new IOException("Failed to read data length");

            int length = (bytes[0] << 24) |
                        (bytes[1] << 16) |
                        (bytes[2] << 8) |
                        bytes[3];

            bytes = new byte[length];
            bytesRec = mClient.Receive(bytes, 0, length, SocketFlags.None);
            string message = Encoding.UTF8.GetString(bytes);

            Console.WriteLine("Server says: {0} (◠‿◠✿)",
                            Encoding.ASCII.GetString(bytes, 0, bytesRec));

            return new Info(code, message);
        }
        public void CloseSocket()
        {
            // Release the socket
            mClient.Shutdown(SocketShutdown.Both);
            mClient.Close();
        }
        public class Info
        {
            public int mCode;
            public string mJson;

            public Info(int code, string json)
            {
                mCode = code;
                mJson = json;
            }
            public byte[] ToByte()
            {
                // Convert message to bytes
                byte[] dataBytes = Encoding.UTF8.GetBytes(mJson);
                int dataLength = dataBytes.Length;

                // Create buffer: 1Byte code + 4Byte length + LengthByte data
                byte[] packet = new byte[1 + 4 + dataLength];

                // Set message code
                packet[0] = (byte)mCode;

                // Set data length
                packet[1] = (byte)(dataLength >> 24);
                packet[2] = (byte)(dataLength >> 16);
                packet[3] = (byte)(dataLength >> 8);
                packet[4] = (byte)dataLength;

                // Copy message data
                Buffer.BlockCopy(dataBytes, 0, packet, 5, dataLength);

                return packet;
            }
        }
    }
}
