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
using Newtonsoft.Json;

namespace Client
{
    /// <summary>
    /// Interaction logic for GameScore.xaml
    /// </summary>
    public partial class GameScore : Window
    {
        User currClient;
        public GameScore(string gameResult, User client)
        {
            InitializeComponent();
            string parsedResults = parse(gameResult);
            Score.Content = parsedResults;
            currClient = new User(client); 
        }

        private string parse(string gameResult)
        {
            const int bracketMinCount = 3;
            string ret = "";
            int bracketCount = 0;
            int firstIndexToRemove = 0;

            while(gameResult[firstIndexToRemove] != '\0')
            {
                firstIndexToRemove++;
            }

            gameResult = gameResult.Remove(firstIndexToRemove);

            for(int i = 0; i < gameResult.Length; i++)
            {
                if (gameResult[i].Equals('{')) bracketCount++;
                else if (gameResult[i].Equals('}'))
                {
                    bracketCount--;
                    ret += '\n';
                }

                if(bracketCount >= bracketMinCount && !gameResult[i].Equals('{') && !gameResult[i].Equals('"'))
                {
                    ret += gameResult[i];
                }
            }

            return ret;
        }

        private void ExitGame_Click(object sender, RoutedEventArgs e)
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "back_to_menu"
            };

            string resp = currClient.GetCommunicator().BackToMenuRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            if (resp.Equals("back_to_menu: success"))
            {
                Main main = new Main(currClient);
                main.Show();
                Close();
            }
        }
    }
}
