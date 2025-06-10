using BackendTrivia;
using static BackendTrivia.Communicator;
using static Trivia.Codes;
using System.Text.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Trivia;
using static Trivia.Responses;
using System.Text.Json.Serialization;

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

        public Menu CloseRoom()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.CLOSE_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

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

            GetRoomStateResponse result = JsonSerializer.Deserialize<GetRoomStateResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_ROOM_STATE_RESPONSE_CODE) && result != null)
            {
                return result;
            }

            throw new Exception();
        }

        // TWO OPTIONS FOR THE FUNCTION - FOR A CERTAIN ROOM WITH ID, OR FOR THE CURRENT ROOM
        public List<string> GetPlayersInRoom(uint roomId)
        {
            var data = new
            {
                roomId = roomId
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_PLAYERS_IN_ROOMS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            GetPlayersInRoomResponse result = JsonSerializer.Deserialize<GetPlayersInRoomResponse>(infoRecvived.mJson);


            if (infoRecvived.mCode == ((int)ResponseCodes.GET_PLAYERS_IN_ROOMS_RESPONSE_CODE) && result != null)
            {
                return result.Players;
            }

            throw new Exception();
        }

        public List<string> GetPlayersInRoom()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_PLAYERS_IN_ROOMS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            GetPlayersInRoomResponse result = JsonSerializer.Deserialize<GetPlayersInRoomResponse>(infoRecvived.mJson);


            if (infoRecvived.mCode == ((int)ResponseCodes.GET_PLAYERS_IN_ROOMS_RESPONSE_CODE) && result != null)
            {
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

            GetRoomsResponse result = JsonSerializer.Deserialize<GetRoomsResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_ROOMS_RESPONSE_CODE) && result != null)
            {
                return result.Rooms;
            }

            throw new Exception();
        }
    }
}
