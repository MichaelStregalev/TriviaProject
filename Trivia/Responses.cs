using System.Collections.Generic;
using static Trivia.Codes;

/*
    RESPONSES
    This file includes all of the
    different types of response structures.
*/
namespace Trivia
{
    internal class Responses
    {
        public struct RoomData
        {
            uint id;
            string name;
            uint maxPlayers;
            uint numOfQuestionsInGame;
            uint timePerQuestion;
        }
        public enum RoomStatus
        {
            RoomStatus
        }
        public struct LoginResponse
        {
            public uint Status;
        }

        public struct SignupResponse
        {
            public uint Status;
        }

        public struct ErrorResponse
        {
            public string Message;
        }

        public struct LogoutResponse
        {
            public uint Status;
        }

        public struct GetRoomsResponse
        {
            public uint Status;
            public List<RoomData> Rooms;
        }

        public struct GetPlayersInRoomResponse
        {
            public List<string> Players;
        }

        public struct GetHighScoreResponse
        {
            public uint Status;
            public List<int> Scores;
        }

        public struct GetStatisticsResponse
        {
            public uint Status;
            public List<double> Statistics;
        }

        public struct JoinRoomResponse
        {
            public uint Status;
        }

        public struct CreateRoomResponse
        {
            public uint Status;
        }

        public struct CloseRoomResponse
        {
            public uint Status;
        }

        public struct StartRoomResponse
        {
            public uint Status;
        }

        public struct LeaveRoomResponse
        {
            public uint Status;
        }

        public struct GetRoomStateResponse
        {
            public uint Status;
            public bool HasGameBegun;
            public List<string> Players;
            public uint AnswerCount;
            public uint AnswerTimeOut;
        }
    }
}