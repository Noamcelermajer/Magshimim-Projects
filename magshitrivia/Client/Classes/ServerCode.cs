using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public static class ServerCodes
    {
        #region Login
        public const string LoginSuccess = "login: success";
        public const string LoginWrong = "login: err_user_doesnt_exist";
        public const string LoginAlreadyConnected = "login: err_user_logged_in";
        public const string LoginFail = "login: err_password_mismatch";
        #endregion // using Regions to make code more easier to read.
        #region SignUp
        public const string SignUpSuccess = "signup: success";
        public const string SignUpPassword = "signup: err_invalid_input_password";
        public const string SignUpEmail = "signup: err_invalid_input_email";
        public const string SignUpUsernameExists = "signup: err_invalid_input_username";
        #endregion
        #region JoinRoom
        public const string JoinRoomMissingParameter = "err_missing_parameters";
        public const string JoinRoomOutOfRange = "err_out_of_range";
        public const string JoinRoomFull = "err_room_full";
        public const string JoinRoomFail = "err_player_already_in_room";
        public const string JoinRoomSuccess = "join_room: success";
        #endregion
        #region Signout
        public const string SignOutSucess = "signout: success";
        #endregion
        #region CreateRoom
        public const string CreateRoomMissingParameter = "create_room: err_missing_parameters";
        public const string CreateRoomSuccess = "create_room: success";
        public const string CreateRoomNameOccupied = "create_room: err_room_name_occupied";
        #endregion
        #region GetPlayerInRoom
        public const string GetPlayerInRoomMissingParameter = "get_players_in_room: err_missing_parameters";
        public const string GetPlayerInRoomOutofRange = "get_players_in_room: err_out_of_range";
        public const string GetPlayerInRoomSuccess = "get_players_in_room: success";
        #endregion
        #region LeaveRoom
        public const string LeaveRoom = "leave_room: success";
        #endregion
    }
}