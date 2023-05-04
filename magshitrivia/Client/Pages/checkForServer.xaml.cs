using System;
using System.IO;
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
    /// Interaction logic for checkForServer.xaml
    /// </summary>
    public partial class checkForServer : Window
    {
        private User client;
        private int port;
        private string ip;

        public checkForServer()
        {
            GetCommunicatorIpAndPort();
            try
            {
                client = new User(new Communicator(ip, port));
                Login l = new Login(client);
                l.Show();
                Close();
            }
            catch
            {
                InitializeComponent();
            }

        }
        private void GetCommunicatorIpAndPort()
        {
            string text = "";
            if (!File.Exists("../../config.txt"))
            {

                string path = System.AppDomain.CurrentDomain.BaseDirectory;
                path = path + "/config.txt";
                text = System.IO.File.ReadAllText(path);
            }
            else
            {
                text = System.IO.File.ReadAllText("../../config.txt");
            }
            string[] ipPort = text.Split(',');
            ip = ipPort[0];
            port = Int32.Parse(ipPort[1]);

        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                client = new User(new Communicator(ip, port));
                Login l = new Login(client);
                l.Show();
                Close();
            }
            catch
            {
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            System.Environment.Exit(1);
        }
    }
}
