using System.Collections.Generic;
using System.Text.Json.Serialization;
using static Trivia.Codes;

/*
    RESPONSES
    This file includes all of the
    different types of response structures.
*/
namespace Trivia
{
    public class Responses
    {
        public class SubmitAnswerResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("correctAnswerId")]
            public uint CorrectAnswerId { get; set; }
        };
        public class GetGameResultsResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("results")]
            public required List<PlayerResult> Results { get; set; }
        };
        public class GetQuestionResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("question")]
            public required string Question { get; set; }

            [JsonPropertyName("answers")]
            public required Dictionary<uint, string> Answers { get; set; }
        };
        public class PlayerResult
        {
            public required string Username { get; set; }
            public uint CorrectAnswerCount { get; set; }
            public uint WrongAnswerCount { get; set; }
            public double AverageAnswerTime { get; set; }
        };

        public class RoomData
        {
            public uint Id { get; set; }
            public required string Name { get; set; }
            public uint MaxPlayers { get; set; }
            public uint NumOfQuestionsInGame { get; set; }
            public uint TimePerQuestion { get; set; }
        }
        public class LoginResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class SignupResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class ErrorResponse
        {
            [JsonPropertyName("message")]
            public required string Message { get; set; }
        }

        public class LogoutResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class GetRoomsResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("rooms")]
            public required List<RoomData> Rooms { get; set; }
        }

        public class GetPlayersInRoomResponse
        {
            [JsonPropertyName("players")]
            public required List<string> Players { get; set; }
        }

        public class GetHighScoreResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("scores")]
            public required List<int> Scores { get; set; }

            [JsonPropertyName("names")]
            public required List<string> Names { get; set; }
        }

        public class GetStatisticsResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("statistics")]
            public required List<double> Statistics { get; set; }
        }

        public class JoinRoomResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class CreateRoomResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("roomId")]
            public uint RoomId { get; set; }
        }

        public class CloseRoomResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class StartRoomResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class LeaveRoomResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }
        }

        public class GetRoomStateResponse
        {
            [JsonPropertyName("status")]
            public uint Status { get; set; }

            [JsonPropertyName("hasGameBegun")]
            public bool HasGameBegun { get; set; }

            [JsonPropertyName("players")]
            public required List<string> Players { get; set; }

            [JsonPropertyName("questionCount")]
            public uint AnswerCount { get; set; }

            [JsonPropertyName("answerTimeout")]
            public uint AnswerTimeOut { get; set; }
        }


    }
}