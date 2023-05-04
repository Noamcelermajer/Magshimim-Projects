using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Newtonsoft;
using System.Threading;
using Newtonsoft.Json;

namespace Client
{
    /// <summary>
    /// Interaction logic for WaitForGameUser.xaml
    /// </summary>
    public partial class WaitForGameUser : Window
    {
        User curUser;
        bool running;
        JsonCreateRoomRequest currRoomState;
        public WaitForGameUser(User user, JsonCreateRoomRequest roomInfo)
        {
            currRoomState = new JsonCreateRoomRequest(roomInfo);
            curUser = new User(user);
            InitializeComponent();
            running = true;
            new Thread(initializeList).Start();
            Information.Text = roomInfo.ToString();
            RoomName.Content += roomInfo.room_name;

        }

        private void ParticipantList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        public void initializeList()
        {
            Dictionary<string, string> PlayerDict = GetRoomInfo();
            while (running)
            {
                if (running)
                {
                    this.Dispatcher.Invoke(new Action(() =>
                    {
                        participantList.Items.Clear();
                    }));
                }

                foreach (KeyValuePair<string, string> entry in PlayerDict)
                {
                    this.Dispatcher.Invoke(new Action(() =>
                    {
                        participantList.Items.Add(entry.Value);
                    }));

                }
                System.Threading.Thread.Sleep(1000);
                if(running) PlayerDict = GetRoomInfo();
            }
        }

        private Dictionary<string, string> GetRoomInfo()
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "get_room_state"
            };
            string roomState = curUser.GetCommunicator().GetRoomStateRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            Response l = JsonConvert.DeserializeObject<Response>(roomState);
            if (l.response.Equals("err_room_shutdown"))
            {
                this.Dispatcher.Invoke(new Action(() =>
                {
                    running = false;
                    Main main = new Main(curUser);
                    main.Show();
                    Close();
                }));
            }
            else if (l.response.Equals("err_game_started"))
            {
                this.Dispatcher.Invoke(new Action(() =>
                {
                    running = false;
                    TriviaGame g = new TriviaGame(curUser);
                    g.Show();
                    Close();
                }));
            }
            
            roomState = GetPlayerInRoom(roomState);
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(roomState);
        }

        public string GetPlayerInRoom(string Information)
        {
            string[] msg = Information.Split(',');
            string response = "";
            string data = "{";
            for (int i = 0; i < msg.Length; i++)
            {
                if (i == 0)
                {
                    response += msg[i];
                    response += '}';
                }
                else
                {
                    data += msg[i];
                }
            }
            string[] playerlst = data.Split(new string[] { "players" }, StringSplitOptions.None);
            if (playerlst.Length >= 2)
            {


                string playerStr = playerlst[1];
                int count1 = 0, count2 = 0, countBrace = 0;
                string newStr = "";
                for (int i = 3; i < playerStr.Length; i++)
                {
                    if (playerStr[i].Equals('\"'))
                    {
                        countBrace++;
                        newStr += playerStr[i];
                        if (countBrace == 4 && !playerStr[i + 1].Equals('}'))
                        {
                            countBrace = 0;
                            newStr += ',';
                        }

                    }
                    else if (playerStr[i].Equals('{'))
                    {
                        count1++;
                        newStr += playerStr[i];
                    }
                    else if (playerStr[i].Equals('}'))
                    {
                        if (count1 > count2)
                        {
                            count2++;
                            newStr += playerStr[i];
                        }
                    }
                    else if (!playerStr[i].Equals('\0'))
                    {
                        newStr += playerStr[i];
                    }
                    else
                    {
                        break;
                    }
                }
                return newStr;
            }
            return "";
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "leave_room"
            };

            if (curUser.GetCommunicator().LeaveRoomRequest(Newtonsoft.Json.JsonConvert.SerializeObject
                (jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
                { NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore, Formatting = Newtonsoft.Json.Formatting.None })).Equals("leave_room: success"))
            {
                running = false;
                Main main = new Main(curUser);
                main.Show();
                Close();
            }
        }
    }
}
