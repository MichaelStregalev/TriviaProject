using BackendTrivia;
using static BackendTrivia.Communicator;
using static Trivia.Codes;
using System.Text.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Trivia;
using static BackendTrivia.Communicator;
using static Trivia.Codes;
using static Trivia.Responses;

namespace BackendTrivia
{
    public class Room
    {
        private Communicator mCom;
        public Room(Communicator c)
        {
            mCom = c;
        }
        public Communicator GetCommunicator()
        {
            return this.mCom;
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

            mCom.Send(((int)RequestCodes.CREATE_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            int status = doc.RootElement.GetProperty("status").GetInt32();

            if (infoRecvived.mCode == ((int)ResponseCodes.CREATE_ROOM_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Room JoinRoom(int RoomId)
        {
            var data = new
            {
                roomId = RoomId
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.JOIN_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            int status = doc.RootElement.GetProperty("status").GetInt32();

            if (infoRecvived.mCode == ((int)ResponseCodes.JOIN_ROOM_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Room CloseRoom()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.CLOSE_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            int status = doc.RootElement.GetProperty("status").GetInt32();

            if (infoRecvived.mCode == ((int)ResponseCodes.CLOSE_ROOM_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Room StartRoom()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.START_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            int status = doc.RootElement.GetProperty("status").GetInt32();

            if (infoRecvived.mCode == ((int)ResponseCodes.START_ROOM_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Room LeaveRoom()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.LEAVE_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            int status = doc.RootElement.GetProperty("status").GetInt32();

            if (infoRecvived.mCode == ((int)ResponseCodes.LEAVE_ROOM_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Room GetRoomState()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_ROOM_STATE_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            Responses.GetRoomStateResponse result = JsonSerializer.Deserialize<GetRoomStateResponse>(infoRecvived.mJson);

            uint status = result.Status;
            bool hasGameBegun = result.HasGameBegun;
            List<string> players = result.Players;
            uint answerCount = result.AnswerCount;
            uint answerTimeOut = result.AnswerTimeOut;

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_ROOM_STATE_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

    }
}
