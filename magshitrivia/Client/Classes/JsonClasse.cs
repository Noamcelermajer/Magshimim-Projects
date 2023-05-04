using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{

    public class JsonSubmitAnswerRequest
    {
        public string type;
        public int answer_id;
        public long time_to_answer;
    }
    public class JsonSignoutRequest
    {
        public string type;
        public string username;
    }
    public class JsonCreateRoomRequest
    {
        public string type;
        public string room_name;
        public int max_users;
        public int question_count;
        public int answer_timeout;
        public JsonCreateRoomRequest()
        {
        }

        public JsonCreateRoomRequest(JsonCreateRoomRequest room)
        {
            type = room.type;
            room_name = room.room_name;
            max_users = room.max_users;
            question_count = room.question_count;
            answer_timeout = room.answer_timeout;
        }

        public override string ToString()
        {
            return "max_user : " + max_users + " question_count : " + question_count + " answer_timeout : " + answer_timeout;
        }
    }
    public class JsonJoinRoomRequest
    {
        public string type;
        public int room_id;
    }
    public class JsonGlobalRequest
    {
        public string type;
    }
    public class JsongetPlayersInRoomRequest
    {
        public string type;
        public int room_id;
    }
    #region Classes
    public class JsonUser
    {
        public string type { get; set; }
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }
    public class Players
    {
        public string username { get; set; }
    }

    public class RoomData
    {
        public int has_game_begun { get; set; }
        public int question_count { get; set; }
        public int answer_timeout { get; set; }
        public List<Players> players { get; set; }
    }

    public partial class JsonQuestionAnswer
    {
        public string Response { get; set; }

        public QuestionAnswerData Data { get; set; }
    }

    public partial class QuestionAnswerData
    {
        public string Question { get; set; }

        public Dictionary<string, string> Answers { get; set; }
    }
    public class JsonRoomState
    {
        public string response { get; set; }
        public RoomData data { get; set; }
    }
    public partial class JsonGameResult
    {
        public string Response { get; set; }

        public Dictionary<string, UserResult> Data { get; set; }

        override
        public string ToString()
        {
            string myStr = "";
            foreach (KeyValuePair<string, UserResult> entry in Data)
            {
                myStr += entry.ToString();
                myStr += "\n";
            }
            return myStr;

        }
    }
    public partial class JsonFullRoomData
    {
        public string Response { get; set; }

        public Dictionary<int, FullRoomData> data { get; set; }
    }

    public partial class FullRoomData
    {
        public string Name { get; set; }

        public int Max_players { get; set; }

        public int Question_count { get; set; }

        public int Question_timeout { get; set; }

        public int Is_Active { get; set; }
        override
        public string ToString()
        {
            return "name : " + Name + ", max_player : " + Max_players + ", " + "question count : " + Question_count + ", question time : " + Question_timeout +  "";

        }
    }

    public partial class UserResult
    {
        public string Username { get; set; }

        public long CorrectAnswers { get; set; }

        public long WrongAnswers { get; set; }

        public long AverageTime { get; set; }

        override
        public string ToString()
        {
            return "Username : " + Username + ", Score : " + CorrectAnswers + ", " + "AverageTime : " + AverageTime + ";";

        }
    }
    #endregion
}
