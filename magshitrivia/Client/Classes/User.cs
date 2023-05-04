using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class Response
    {
        public string response;
    }

    public static class Session
    {
        public static User CurrentUser = null;
        public static bool Logged = false;
        public static bool JustSignedUp = false;
    }


    public class User
    {
        public string username;
        public string password;
        public string email;
        private Communicator client;
        private Room _currRoom;


        public User()
        {
            username = null;
            password = null;
            email = null;
            client = null;
            _currRoom = null;
        }
        public User(string Username, string Password, string Type, string Email = null)
        {
            this.username = Username;
            this.password = Password;
            email = Email;
        }

        public User(User other)
        {
            username = other.username;
            password = other.password;
            email = other.email;
            client = other.client;
        }
        public User(Communicator other)
        {
            username = null;
            password = null;
            email = null;
            _currRoom = null;
            client = other;
        }

        public void SetInformation(string Username, string Password, string Type, string Email = null)
        {
            this.username = Username;
            this.password = Password;
            email = Email;
        }
        public void SetCommunicator(Communicator client) => this.client = client;
        public void SetEmail(string email) => this.email = email;

        public Communicator GetCommunicator() => client;


        public Room GetRoom() => _currRoom;

        public string GetUsername() => username;


        public class Room
        {
            public int MaxUsers { get; set; }
            public int QuestionNumber { get; set; }
            public int QuestionTime { get; set; }
            public List<string> Users { get; private set; }
            public bool IsAdmin { get; set; }
            public int Id { get; set; }
            public string RoomName { get; set; }

            public Room(int QuestionNumber, int MaxUsers, int QuestionTime, bool IsAdmin, string RoomName)
            {
                this.QuestionNumber = QuestionNumber;
                this.QuestionTime = QuestionTime;
                this.MaxUsers = MaxUsers;
                this.IsAdmin = IsAdmin;
                this.RoomName = RoomName;
            }

            public void SetUsers(List<string> other)
            {
                this.Users = new List<string>(other);
            }
        }
    }
}
