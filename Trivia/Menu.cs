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
using System.Windows.Controls;

namespace BackendTrivia
{
    public class Menu
    {
        private Communicator mCom;
        public Menu(Communicator c)
        {
            mCom = c;
        }
        public Communicator GetCommunicator()
        {
            return mCom;
        }
        public (Room, int) CreateRoom(string RoomName, int MaxPlayers, int QuestionCount, int AnswerTimeOut)
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

            if (infoRecvived.mCode == ((int)ResponseCodes.CREATE_ROOM_RESPONSE_CODE))
            {
                // Only deserialize on success
                Responses.CreateRoomResponse result = JsonSerializer.Deserialize<Responses.CreateRoomResponse>(infoRecvived.mJson);
                int roomId = result.RoomId;
                return (new Room(mCom), roomId);
            }

            // Handle error case
            string errorMessage = "An unknown error occurred during room creation.";

            JsonDocument doc = JsonDocument.Parse(infoRecvived.mJson);
            JsonElement root = doc.RootElement;
            if (root.TryGetProperty("message", out JsonElement messageElement))
            {
                errorMessage = messageElement.GetString();
            }

            throw new Exception(errorMessage);
        }

        public Room JoinRoom(int RoomId)
        {
            var data = new
            {
                roomid = RoomId
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.JOIN_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.JOIN_ROOM_RESPONSE_CODE))
            {
                return new Room(mCom);
            }

            throw new Exception();
        }

        public List<double> Statistics()
        {

            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_STATISTICS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            Responses.GetStatisticsResponse result = JsonSerializer.Deserialize<GetStatisticsResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_STATISTICS_RESPONSE_CODE) && result != null && result.Statistics != null)
            {
                return result.Statistics;
            }

            throw new Exception();
        }

        public List<(string, int)> HighScores()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_HIGHSCORE_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            Responses.GetHighScoreResponse result = JsonSerializer.Deserialize<GetHighScoreResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_HIGHSCORE_RESPONSE_CODE))
            {
                var highscores = new List<(string, int)>();
                for (int i = 0; i < result.Scores.Count; i++)
                {
                    highscores.Add((result.Names[i], result.Scores[i]));
                }
                return highscores;
            }

            throw new Exception();
        }

        public void Logout()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.LOGOUT_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode != ((int)ResponseCodes.LOGOUT_RESPONSE_CODE))
            {
                throw new Exception();
            }
        }
    }
}
