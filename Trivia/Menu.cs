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

            mCom.Send(((int)RequestCodes.CREATE_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.CREATE_ROOM_RESPONSE_CODE))
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

            mCom.Send(((int)RequestCodes.JOIN_ROOM_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.JOIN_ROOM_RESPONSE_CODE))
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

            mCom.Send(((int)RequestCodes.GET_STATISTICS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            Responses.GetStatisticsResponse result = JsonSerializer.Deserialize<GetStatisticsResponse>(infoRecvived.mJson);

            uint status = result.Status;
            List<double> stats = result.Statistics;

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_STATISTICS_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }

        public Menu HighScores()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_HIGHSCORE_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            Responses.GetHighScoreResponse result = JsonSerializer.Deserialize<GetHighScoreResponse>(infoRecvived.mJson);

            uint status = result.Status;
            List<int> stats = result.Scores;

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_HIGHSCORE_RESPONSE_CODE))
            {
                return this;
            }

            throw new Exception();
        }
    }
}
