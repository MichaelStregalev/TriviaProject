using Trivia;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Trivia
{
    internal class Codes
    {
        public enum ResponseCodes
        {
            ERROR_RESPONSE_CODE = 0,
            SIGNUP_RESPONSE_CODE = 20,
            LOGIN_RESPONSE_CODE = 21,
            LOGOUT_RESPONSE_CODE = 31,
            GET_ROOMS_RESPONSE_CODE = 23,
            GET_PLAYERS_IN_ROOMS_RESPONSE_CODE = 22,
            JOIN_ROOM_RESPONSE_CODE = 32,
            CLOSE_ROOM_RESPONSE_CODE = 33,
            START_ROOM_RESPONSE_CODE = 34,
            LEAVE_ROOM_RESPONSE_CODE = 35,
            GET_ROOM_STATE_RESPONSE_CODE = 36,
            CREATE_ROOM_RESPONSE_CODE = 42,
            GET_HIGHSCORE_RESPONSE_CODE = 52,
            GET_STATISTICS_RESPONSE_CODE = 62,
            GET_GAME_RESULTS_RESPONSE_CODE = 71, 
            SUBMIT_ANSWER_RESPONSE_CODE = 72,
            GET_QUESTION_RESPONSE_CODE = 73, 
            LEAVE_GAME_RESPONSE_CODE = 74
        }

        public enum RequestCodes
        {
            SIGNUP_REQUEST_CODE = 1,
            LOGIN_REQUEST_CODE,
            LOGOUT_REQUEST_CODE,
            GET_ROOMS_REQUEST_CODE,
            GET_PLAYERS_IN_ROOMS_REQUEST_CODE,
            JOIN_ROOM_REQUEST_CODE,
            CLOSE_ROOM_REQUEST_CODE,
            START_ROOM_REQUEST_CODE,
            LEAVE_ROOM_REQUEST_CODE,
            GET_ROOM_STATE_REQUEST_CODE,
            CREATE_ROOM_REQUEST_CODE,
            GET_HIGHSCORE_REQUEST_CODE,
            GET_STATISTICS_REQUEST_CODE,
            GET_GAME_RESULTS_REQUEST_CODE, 
            SUBMIT_ANSWER_REQUEST_CODE,
            GET_QUESTION_REQUEST_CODE, 
            LEAVE_GAME_REQUEST_CODE
        }

        public const int SUCCESSFUL_LOGIN = 99;
        public const int SUCCESSFUL_SIGNUP = 100;


        // <-- CONSTS TO PREVENT MAGIC NUMBERS -->

        public const int REQUEST_ID_IN_BUFFER = 0;
        public const int DATA_LENGTH_START = 1;
        public const int DATA_LENGTH_END = 4;
        public const int DATA_START = 5;

        // STATISTICS CONSTS
        public const int STATS_COUNT = 5;
        public const int AVGANSWERTIME_INDEX = 0;
        public const int CORRECT_ANSWERS_INDEX = 1;
        public const int TOTAL_ANSWERS_INDEX = 2;
        public const int TOTAL_GAMES_INDEX = 3;
        public const int SCORE_INDEX = 4;

        // JOIN ROOM CONSTS
        public const int MAX_FONT_SIZE_PLAYER = 20;
        public const double MULTIPLICATION_FONT = 0.6;
        public const int BIGGER_FONT_ADDITION = 2;

        public enum RoomStatus
        {
            ROOM_OPEN = 200,
            GAME_STARTED,
            GAME_ENDED
        }
        public enum GameCodes 
        { 
            MORE_QUESTIONS = 90, 
            NO_MORE_QUESTIONS
        }
    }
}