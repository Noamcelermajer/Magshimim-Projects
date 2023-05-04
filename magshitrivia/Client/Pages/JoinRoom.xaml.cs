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
using System.Threading;
using Newtonsoft;

namespace Client
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Window
    {
        private JsonFullRoomData roomsData;
        private User curUser;
        private JsonCreateRoomRequest roomInformation;
        private bool running;
        private const int getRoomsDataBeginNum = 2;
        private string lastClicked;

        public JoinRoom(User user)
        {
            roomInformation = new JsonCreateRoomRequest();
            curUser = new User(user);
            InitializeComponent();
            running = true;
            new Thread(initializeList).Start();
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            Main main = new Main(curUser);
            running = false;
            main.Show();
            Close();
        }

        private void RoomList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                string selectedItem = RoomList.Items[RoomList.SelectedIndex].ToString();
                lastClicked = selectedItem;
            }
            catch
            {
                JsonJoinRoomRequest joinRoom = new JsonJoinRoomRequest { type = "join_room", room_id = int.Parse("" + lastClicked[0]) };
                string joinRoomResponse = curUser.GetCommunicator().JoinRoomRequest(Newtonsoft.Json.JsonConvert.SerializeObject(joinRoom, new Newtonsoft.Json.JsonSerializerSettings
                {
                    NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                    Formatting = Newtonsoft.Json.Formatting.None
                }));

                if (joinRoomResponse.Equals(ServerCodes.JoinRoomSuccess))
                {
                    ParseRoomState(int.Parse("" + lastClicked[0]));
                    WaitForGameUser wfgu = new WaitForGameUser(curUser, roomInformation);
                    running = false;
                    wfgu.Show();
                    Close();
                }

                else
                {
                    MessageBox.Show("Error joining room");
                }
            }
        }

        private JsonFullRoomData GetRooms()
        {
            JsonGlobalRequest getRooms = new JsonGlobalRequest { type = "get_rooms" };
            string roomInfo = curUser.GetCommunicator().GetRoomsRequest(Newtonsoft.Json.JsonConvert.SerializeObject(getRooms, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));
            roomsData = Newtonsoft.Json.JsonConvert.DeserializeObject<JsonFullRoomData>(roomInfo);
            return roomsData;
        }

        private void ParseRoomState(int id)
        {
            foreach (KeyValuePair<int, FullRoomData> entry in roomsData.data)
            {
                if (entry.Key == id)
                {
                    roomInformation.answer_timeout = (entry.Value.Question_timeout);
                    roomInformation.max_users = entry.Value.Max_players;
                    roomInformation.question_count = entry.Value.Question_count;
                    roomInformation.room_name = entry.Value.Name;
                }
            }
        }
        private void initializeList()
        { 
            Dictionary<int, string> info = new Dictionary<int, string>();

            while (running)
            {
                if(running)
                {
                    this.Dispatcher.Invoke(new Action(() =>
                    {
                        if (RoomList.Items.Count != 0)
                            RoomList.Items.Clear();
                    }));
                }



                JsonFullRoomData rmData = GetRooms();
                if(rmData.Response != "invalid")
                {
                    foreach (KeyValuePair<int, FullRoomData> entry in rmData.data)
                    {
                        this.Dispatcher.Invoke(new Action(() =>
                        {
                            RoomList.Items.Add(entry.Key.ToString() + ") " + entry.Value.ToString());
                        }));
                    }
                }


                System.Threading.Thread.Sleep(1000);
            }
        }
    }
}
