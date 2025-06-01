using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using static BackendTrivia.Communicator;

namespace BackendTrivia
{
    public class Menu
    {
        private Communicator mCom;
        public Menu(Communicator c) 
        {
            mCom = c;
        }

        public Room CreateRoom(string RoomName, int MaxPlayers, int QuestionCount, int AnswerTimeOut)
        {
            var data = new
            {
                roomName = RoomName,
                maxPlayers = MaxPlayers,
                questionCount = QuestionCount,
                answerTimeout = AnswerTimeOut
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(7, json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == 42)
            {
                return new Room(mCom);
            }

            throw new Exception();
        }

        public Room JoinRoom(int RoomId)
        {
            var data = new
            {
                roomid = RoomId
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(6, json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == 32)
            {
                return new Room(mCom);
            }

            throw new Exception();
        }

        public Menu Statistics()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(9, json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == 62)
            {
                return this;
            }

            throw new Exception();
        }
    }
}
