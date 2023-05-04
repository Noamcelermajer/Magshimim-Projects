using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Newtonsoft.Json;

namespace Client
{
    /// <summary>
    /// Interaction logic for WaitForGame.xaml
    /// </summary>
    public partial class WaitForGameAdmin : Window
    {
        private User CurrentClient;
        private JsonCreateRoomRequest roomInformation;
        private bool running;

        public WaitForGameAdmin(User client, JsonCreateRoomRequest CurrentRoomInfo)
        {
            CurrentClient = new User(client);
            roomInformation = new JsonCreateRoomRequest(CurrentRoomInfo);
            InitializeComponent();
            running = true;
            new Thread(initializeList).Start();
            information.Text = roomInformation.ToString();
            RoomName.Content += roomInformation.room_name;
        }

        private void initializeList()
        {
            Dictionary<string, string> PlayerDict = GetRoomInfo();
            while (running)
            {

                this.Dispatcher.Invoke(new Action(() =>
                {
                    participantList.Items.Clear();
                }));
                foreach (KeyValuePair<string, string> entry in PlayerDict)
                {
                    this.Dispatcher.Invoke(new Action(() =>
                    {
                        participantList.Items.Add(entry.Value);
                    }));

                }
                System.Threading.Thread.Sleep(1000);
                if (running) PlayerDict = GetRoomInfo();
            }
        }
        private void ParticipantList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
        }
        private Dictionary<string, string> GetRoomInfo()
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "get_room_state"
            };
            string roomState = CurrentClient.GetCommunicator().GetRoomStateRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));
            roomState = GetPlayerInRoom(roomState);
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(roomState);
        }
        private string GetPlayerInRoom(string Information)
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            new Thread(initializeList).Start();
        }

        private void CloseGame_Click(object sender, RoutedEventArgs e)
        {
            JsonGlobalRequest closeRoom = new JsonGlobalRequest
            {
                type = "close_room"
            };
            string response = CurrentClient.GetCommunicator().CloseRoomRequest(Newtonsoft.Json.JsonConvert.SerializeObject(closeRoom, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            if (response.Equals("close_room: success"))
            {
                running = false;
                Main m = new Main(CurrentClient);
                m.Show();
                Close();
            }
        }
        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            JsonGlobalRequest request = new JsonGlobalRequest()
            {
                type = "start_game"
            };
            string response = CurrentClient.GetCommunicator().CloseRoomRequest(Newtonsoft.Json.JsonConvert.SerializeObject(request, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            if (response.Equals("start_game: success"))
            {
                running = false;
                TriviaGame m = new TriviaGame(CurrentClient);
                m.Show();
                Close();
            }
        }
    }
}
