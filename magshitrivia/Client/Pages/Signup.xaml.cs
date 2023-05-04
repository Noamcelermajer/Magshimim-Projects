using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Data;
using System.Data.SqlClient;
using System.Text.RegularExpressions;
using System.Net;
using Client;
namespace Client
{
    /// <summary>  
    /// Interaction logic for Registration.xaml  
    /// </summary>  
    public partial class Signup : Window
    {
        User client;
        public Signup()
        {
            InitializeComponent();
        }
        public Signup(User currentClient)
        {
            InitializeComponent();
            client = currentClient;
        }
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            Login login = new Login(client);
            login.Show();
            Close();
        }
        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            string inputEmail = email.Text.Replace("\r", "").Replace("\n", "");
            string inputPassword = password.Text.Replace("\n", "").Replace("\r", "");
            string inputUsername = username.Text.Replace("\n", "").Replace("\r", "");
            if (isValid(inputEmail))
            {
                if (inputPassword == ConfirmPassword.Text.Replace("\n", "").Replace("\r", ""))
                {
                    if (inputEmail.Length != 0)
                    {
                        if (inputPassword.Length == 0)
                        {
                            errormessage.Text = "Enter password.";
                            password.Focus();
                        }
                        else
                        {
                            client.SetInformation(username.Text, inputPassword, "signup", email.Text);
                            JsonUser jsonUser = new JsonUser
                            {
                                type = "signup",
                                username = inputUsername,
                                password = inputPassword,
                                email = inputEmail
                            };

                            KeyValuePair<bool, string> result = client.GetCommunicator().SignupRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonUser, new Newtonsoft.Json.JsonSerializerSettings
                            {
                                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                                Formatting = Newtonsoft.Json.Formatting.None
                            }));

                            if (result.Key)
                            {
                                Main login = new Main(client);
                                login.Show();
                                Close();
                            }
                            else
                            {
                                MessageBox.Show(result.Value);
                            }
                        }
                    }
                    else
                    {
                        errormessage.Text = "Enter an email.";
                        email.Focus();
                    }
                }
                else
                {
                    errormessage.Text = "Both passwords must be the same!";
                    password.Focus();

                }
            }
            else
            {
                errormessage.Text = "Enter a valid email!";
                email.Focus();
            }
        }

        private bool isValid(string email)
        {
            return Regex.IsMatch(email, @"^([\w-.]+)@(([[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.)|(([\w-]+.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(]?)$");
        }

        private void Password_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}