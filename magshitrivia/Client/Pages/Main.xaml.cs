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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Main : Window
    {
        User CurrentClient;

        public Main(User client)
        {
            InitializeComponent();
            CurrentClient = new User(client);
        }


        private void Signout_Click(object sender, RoutedEventArgs e)
        {
            if (CurrentClient.GetCommunicator().SignoutRequest(CurrentClient.GetUsername()))
            {
                Login login = new Login(CurrentClient);
                login.Show();
                Close();
            }
        }
        private void Status_Click(object sender, RoutedEventArgs e)
        {
            Status status = new Status(CurrentClient);
            status.Show();
            Close();
        }
        private void BestScores_Click(object sender, RoutedEventArgs e)
        {
            BestScores bestScores = new BestScores(CurrentClient);
            bestScores.Show();
            Close();
        }
        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom jr = new JoinRoom(CurrentClient);
            jr.Show();
            Close();
        }
        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom createRoom = new CreateRoom(CurrentClient);
            createRoom.Show();
            Close();
        }
    }
}
