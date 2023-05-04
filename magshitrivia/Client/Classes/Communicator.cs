using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;


namespace Client
{
    public class Communicator
    {
        IPAddress ipAddr;
        IPEndPoint localEndPoint;
        Socket sender;

        public Communicator(string Ip, int port)
        {
            ipAddr = System.Net.IPAddress.Parse(Ip);
            localEndPoint = new IPEndPoint(ipAddr, port);

            // Creation TCP/IP Socket using  
            // Socket Class Costructor 
            sender = new Socket(ipAddr.AddressFamily,
                       SocketType.Stream, ProtocolType.Tcp);
            try
            {
                sender.Connect(localEndPoint);
            }
            catch
            {
                throw;
            }
        }
        public string EscapeStringValue(string value)
        {

            StringBuilder output = new StringBuilder(value.Length);
            for (int i = 0; i < value.Length; i++)
            {
                switch (value[i])
                {
                    case '\r':
                        break;

                    case '\n':
                        break;

                    default:
                        output.Append(value[i]);
                        break;
                }
            }

            return output.ToString();
        }
        public KeyValuePair<bool, string> SignupRequest(string Information)
        {
            Information.Replace("\n", "");
            Information.Replace("\r", "");
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string jsonCode = System.Text.Encoding.ASCII.GetString(messageReceived);
            Response jsonResp = JsonConvert.DeserializeObject<Response>(jsonCode);
            KeyValuePair<bool, string> ret = new KeyValuePair<bool, string>(SignupServerMessage(messageReceived), jsonResp.response);
            return ret;
        }
        public KeyValuePair<bool, string> LoginRequest(string Information)
        {
            Information.Replace("\n", "");
            Information.Replace("\r", "");
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string jsonCode = System.Text.Encoding.ASCII.GetString(messageReceived);
            Response l = JsonConvert.DeserializeObject<Response>(jsonCode);
            KeyValuePair<bool, string> ret = new KeyValuePair<bool, string>(LoginServerMessage(messageReceived), l.response);
            return ret;
        }
        public JsonQuestionAnswer GetQuestionAnswer(string Information)
        {
            Information.Replace("\n", "");
            Information.Replace("\r", "");
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recv = Encoding.ASCII.GetString(messageReceived);
            return (JsonConvert.DeserializeObject<JsonQuestionAnswer>(recv));
        }
        public bool LoginServerMessage(byte[] msg)
        {
            string jsonCode = System.Text.Encoding.ASCII.GetString(msg);
            Response l = JsonConvert.DeserializeObject<Response>(jsonCode);

            switch (l.response)
            {
                case ServerCodes.LoginFail:
                    return false;
                case ServerCodes.LoginWrong:
                    return false;
                case ServerCodes.LoginAlreadyConnected:
                    return false;
                case ServerCodes.LoginSuccess:
                    return true;
                default:
                    return false;
            }
        }
        public bool SignupServerMessage(byte[] msg)
        {
            string jsonCode = System.Text.Encoding.ASCII.GetString(msg);
            Response s = JsonConvert.DeserializeObject<Response>(jsonCode);

            switch (s.response)
            {
                case ServerCodes.SignUpEmail:
                    return false;
                case ServerCodes.SignUpPassword:
                    return false;
                case ServerCodes.SignUpUsernameExists:
                    return false;
                case ServerCodes.SignUpSuccess:
                    return true;
                default:
                    return false;
            }
        }

        public bool SignoutRequest(string Information)
        {
            JsonSignoutRequest value = new JsonSignoutRequest
            {
                type = "signout",
                username = Information
            };
            string request = Newtonsoft.Json.JsonConvert.SerializeObject(value, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            });
            byte[] messageSent = Encoding.ASCII.GetBytes(request);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            if (JsonConvert.DeserializeObject<Response>(System.Text.Encoding.ASCII.GetString(messageReceived)).response == ServerCodes.SignOutSucess)
            {
                return true;
            }
            return false;

        }

        public Dictionary<bool, string> CreateRoomRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string responseCode = System.Text.Encoding.ASCII.GetString(messageReceived);
            Dictionary<bool, string> ret = new Dictionary<bool, string>();
            string resp = JsonConvert.DeserializeObject<Response>(responseCode).response;
            ret.Add(resp.Equals(ServerCodes.CreateRoomSuccess), resp);
            return ret;
        }

        public string GetRoomStateRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }

        public string LeaveRoomRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            Response l = JsonConvert.DeserializeObject<Response>(recvmsg);
            return l.response;
        }

        public string GetRoomsRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }

        public string JoinRoomRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            Response l = JsonConvert.DeserializeObject<Response>(recvmsg);
            return l.response;
        }

        public string CloseRoomRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            Response l = JsonConvert.DeserializeObject<Response>(recvmsg);
            return l.response;
        }
        public string GetGameResults(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }
        public string SubmitAnswerRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }
        public string BackToMenuRequest(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            Response l = JsonConvert.DeserializeObject<Response>(recvmsg);
            return l.response;
        }
        public string GetHighScores(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }
        public string GetUserStats(string Information)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes(Information);
            int byteSent = sender.Send(messageSent);
            byte[] messageReceived = new byte[1024];
            int byteRecv = sender.Receive(messageReceived);
            string recvmsg = Encoding.ASCII.GetString(messageReceived);
            return recvmsg;
        }
    }
}
