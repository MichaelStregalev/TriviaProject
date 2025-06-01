using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Trivia
{
    internal class Codes
    {
        // RESPONSE CODES
        public enum ResponseCodes
        {
            ERROR_RESPONSE_CODE = 0,
            SIGNUP_RESPONSE_CODE = 11,
            LOGIN_RESPONSE_CODE = 21,
            LOGOUT_RESPONSE_CODE = 31,
            GET_ROOMS_RESPONSE_CODE = 12,
            GET_PLAYERS_IN_ROOMS_RESPONSE_CODE = 22,
            JOIN_ROOM_RESPONSE_CODE = 32,
            CREATE_ROOM_RESPONSE_CODE = 42,
            GET_HIGHSCORE_RESPONSE_CODE = 52,
            GET_STATISTICS_RESPONSE_CODE = 62
        }

        // REQUEST CODES
        public enum RequestCodes
        {
            SIGNUP_REQUEST_CODE = 1,
            LOGIN_REQUEST_CODE,
            LOGOUT_REQUEST_CODE,
            GET_ROOMS_REQUEST_CODE,
            GET_PLAYERS_IN_ROOMS_REQUEST_CODE,
            JOIN_ROOM_REQUEST_CODE,
            CREATE_ROOM_REQUEST_CODE,
            GET_HIGHSCORE_REQUEST_CODE,
            GET_STATISTICS_REQUEST_CODE
        }

        // HANDLERS & LOGIN MANAGER
        public const int SUCCESSFUL_LOGIN = 99;
        public const int SUCCESSFUL_SIGNUP = 100;

        // INDEXES IN BUFFER
        public const int REQUEST_ID_IN_BUFFER = 0;
        public const int DATA_LENGTH_START = 1;
        public const int DATA_LENGTH_END = 4;
        public const int DATA_START = 5;

        // ROOM CODES
        public enum RoomStatus
        {
            ROOM_OPEN = 200
        }
    }
}
