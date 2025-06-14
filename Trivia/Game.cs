using BackendTrivia;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using static BackendTrivia.Communicator;
using static Trivia.Codes;
using static Trivia.Responses;

namespace Trivia
{
    internal class Game
    {
        private Communicator mCom;
        public Game(Communicator c)
        {
            mCom = c;
        }
        public Communicator GetCommunicator()
        {
            return this.mCom;
        }

        public List<PlayerResult> GetGameResults()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_GAME_RESULTS_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            GetGameResultsResponse result = JsonSerializer.Deserialize<GetGameResultsResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_GAME_RESULTS_RESPONSE_CODE) && result != null)
            {
                return result.results;
            }

            throw new Exception();
        }

        public int SubmitAnswer(uint answerId, double answerTime)
        {
            var data = new
            {
                answerId = answerId,
                answerTime = answerTime
            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.SUBMIT_ANSWER_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            SubmitAnswerResponse result = JsonSerializer.Deserialize<SubmitAnswerResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.SUBMIT_ANSWER_RESPONSE_CODE) && result != null)
            {
                return (int)result.correctAnswerId;
            }

            throw new Exception();
        }

        public GetQuestionResponse GetQuestion()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.GET_QUESTION_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            GetQuestionResponse result = JsonSerializer.Deserialize<GetQuestionResponse>(infoRecvived.mJson);

            if (infoRecvived.mCode == ((int)ResponseCodes.GET_QUESTION_RESPONSE_CODE) && result != null)
            {
                return result;
            }

            throw new Exception();
        }

        public void LeaveGame()
        {
            var data = new
            {

            };

            // Serialize to JSON
            string json = JsonSerializer.Serialize(data, new JsonSerializerOptions { });

            mCom.Send(((int)RequestCodes.LEAVE_GAME_REQUEST_CODE), json);

            Info infoRecvived = mCom.Recv();

            if (infoRecvived.mCode == ((int)ResponseCodes.LEAVE_GAME_RESPONSE_CODE))
            {
                return;
            }

            throw new Exception();
        }
    }
}
