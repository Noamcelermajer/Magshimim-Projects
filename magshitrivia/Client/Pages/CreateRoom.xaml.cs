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

namespace Client
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Window
    {
        User CurrentClient;
        public CreateRoom(User client)
        {
            InitializeComponent();
            CurrentClient = new User(client);
        }

        private void back_click(object sender, RoutedEventArgs e)
        {
            Main m = new Main(CurrentClient);
            m.Show();
            Close();
        }

        private void reset_click(object sender, RoutedEventArgs e)
        {
            RoomNameText.Text = "";
            TimeForQuestionText.Text = "";
            NumberOfQuestionText = null;
            NumberOfPlayerText = null;
        }

        private void Send_click(object sender, RoutedEventArgs e)
        {
            string inputRoomName = RoomNameText.Text.Replace("\r", "").Replace("\n", "");
            string inputTimeForQuestion = TimeForQuestionText.Text.Replace("\n", "").Replace("\r", "");
            string inputNumberOfQuestion = NumberOfQuestionText.Text.Replace("\n", "").Replace("\r", "");
            string inputNumberOfPlayerText = NumberOfPlayerText.Text.Replace("\n", "").Replace("\r", "");
            try
            {
                JsonCreateRoomRequest jsonCreateRoom = new JsonCreateRoomRequest
                {
                    type = "create_room",
                    room_name = inputRoomName,
                    max_users = Int32.Parse(inputNumberOfPlayerText),
                    question_count = Int32.Parse(inputNumberOfQuestion),
                    answer_timeout = Int32.Parse(inputTimeForQuestion)
                };

                string jsonRequest = Newtonsoft.Json.JsonConvert.SerializeObject(jsonCreateRoom, new Newtonsoft.Json.JsonSerializerSettings
                {
                    NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore
                });

                Dictionary<bool, string> result = CurrentClient.GetCommunicator().CreateRoomRequest(jsonRequest);
                foreach (KeyValuePair<bool, string> entry in result)
                {
                    if(entry.Key)
                    {
                        WaitForGameAdmin w = new WaitForGameAdmin(CurrentClient, jsonCreateRoom);
                        w.Show();
                        Close();
                    }
                    else
                    {
                        MessageBox.Show(entry.Value);
                    }
                }
            }

            catch
            {
                MessageBox.Show("Bad Input!");
            }
        }

        private void TextBoxRoom_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBoxPlayer_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBoxTime_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
        private void TextBoxText_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}