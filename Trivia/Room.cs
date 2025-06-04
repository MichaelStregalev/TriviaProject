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
        public struct RoomData
        {
            uint id;
            string name;
            uint maxPlayers;
            uint numOfQuestionsInGame;
            uint timePerQuestion;
        }
        public struct GetRoomStateResponse
        {
            public uint Status;
            public bool HasGameBegun;
            public List<string> Players;
            public uint AnswerCount;
            public uint AnswerTimeOut;
        }
        public struct GetPlayersInRoomResponse
        {
            public List<string> Players;
        }

        public struct GetRoomsResponse
        {
            public uint Status;
            public List<RoomData> Rooms;
        }

        private Communicator mCom;
        public Room(Communicator c)
        {
            mCom = c;
        }
        public Communicator GetCommunicator()
        {
            return this.mCom;
        }

        public Menu CloseRoom()
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
                return new Menu(mCom);
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

        public Menu LeaveRoom()
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
                return new Menu(mCom);
            }

            throw new Exception();
        }

        public GetRoomStateResponse GetRoomState()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_ROOM_STATE_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_ROOM_STATE_RESPONSE_CODE))
            {
                GetRoomStateResponse result = JsonSerializer.Deserialize<GetRoomStateResponse>(infoRecvived.mJson);

                uint status = result.Status;
                bool hasGameBegun = result.HasGameBegun;
                List<string> players = result.Players;
                uint answerCount = result.AnswerCount;
                uint answerTimeOut = result.AnswerTimeOut;

                return result;
            }

            throw new Exception();
        }

        public List<string> GetPlayersInRoom(int id)
        {
            var data = new
            {
                id = id
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_PLAYERS_IN_ROOMS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_PLAYERS_IN_ROOMS_RESPONSE_CODE))
            {
                GetPlayersInRoomResponse result = JsonSerializer.Deserialize<GetPlayersInRoomResponse>(infoRecvived.mJson);

                return result.Players;
            }

            throw new Exception();
        }

        public List<RoomData> GetRooms()
        {
            var data = new
            {
                
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_ROOMS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_ROOMS_RESPONSE_CODE))
            {
                GetRoomsResponse result = JsonSerializer.Deserialize<GetRoomsResponse>(infoRecvived.mJson);

                return result.Rooms;
            }

            throw new Exception();
        }
    }
}
