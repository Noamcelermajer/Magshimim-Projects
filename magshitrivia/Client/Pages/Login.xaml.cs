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
    public partial class Login : Window
    {
        User client;
        public Login()
        {
            InitializeComponent();
            client = new User(new Communicator("127.0.0.1", 54000));
        }
        public Login(User currentClient)
        {
            InitializeComponent();
            client = currentClient;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            //Login button
            client.SetInformation(username.Text, password.Text, "login");
            JsonUser jsonUser = new JsonUser
            {
                type = "login",
                username = username.Text,
                password = password.Text,
            };
            KeyValuePair<bool, string> result = client.GetCommunicator().LoginRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonUser, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));
            if (result.Key)
            {
                Main main = new Main(client);
                main.Show();
                Close();
            }
            else
            {
                MessageBox.Show(result.Value);
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            //signup button
            Signup s = new Signup(client);
            s.Show();
            Close();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {

        }
    }
}
