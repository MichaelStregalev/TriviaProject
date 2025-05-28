using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using static BackendTrivia.Communicator;

namespace BackendTrivia
{
    public class Login
    {
        private Communicator mCom;
        public Login() 
        {
            mCom = new Communicator();
        }

        public Menu LoginAcc(string Username, string Password)
        {
            var data = new
            {
                userName = Username,
                password = Password
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(2, json);

            Info infoRecvived = mCom.Recv();

            if(infoRecvived.mCode == 99)
            {
                return new Menu(mCom);
            }

            throw new Exception();
        }
        public Menu signup(string Username, string Password, string Email)
        {
            var data = new
            {
                userName = Username,
                password = Password,
                email = Email
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(1, json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == 100)
            {
                return new Menu(mCom);
            }

            throw new Exception(infoRecvived.mJson);
        }
    }
}
