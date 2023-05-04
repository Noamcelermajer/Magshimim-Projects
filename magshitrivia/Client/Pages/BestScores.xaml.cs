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

namespace Client
{
    /// <summary>
    /// Interaction logic for BestScores.xaml
    /// </summary>
    public partial class BestScores : Window
    {
        User currentClient;
        public BestScores(User client)
        {
            currentClient = new User(client);
            InitializeComponent();
            InitializeHighScore();
        }

        public void InitializeHighScore()
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "get_highscores"
            };

            string highScores = currentClient.GetCommunicator().GetHighScores(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            Score.Content = parse(highScores);
        }

        private string parse(string highScores)
        {
            const int bracketMinCount = 2;
            string ret = "";
            int bracketCount = 0;
            int firstIndexToRemove = 0;

            while (highScores[firstIndexToRemove] != '\0')
            {
                firstIndexToRemove++;
            }

            highScores = highScores.Remove(firstIndexToRemove);

            for (int i = 0; i < highScores.Length; i++)
            {
                if (highScores[i].Equals('{')) bracketCount++;
                else if (highScores[i].Equals('}')) bracketCount--;

                if (bracketCount >= bracketMinCount && !highScores[i].Equals('"') && ((highScores[i - 1].Equals(':') && highScores[i].Equals(' ')) || !highScores[i].Equals(' ')) && !highScores[i].Equals('{'))
                {
                    if (!highScores[i].Equals(',')) ret += highScores[i];
                    else ret += '\n';
                }
            }

            return ret;
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            Main main = new Main(currentClient);
            main.Show();
            Close();
        }
    }
}
