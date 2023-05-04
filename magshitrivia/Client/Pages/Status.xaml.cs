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
    /// Interaction logic for Status.xaml
    /// </summary>
    public partial class Status : Window
    {
        User currentClient;

        public Status(User client)
        {
            currentClient = new User(client);
            InitializeComponent();
            InitializeStats();
        }

        private void InitializeStats()
        {
            JsonGlobalRequest jsonRequest = new JsonGlobalRequest
            {
                type = "get_user_stats"
            };

            string stats = currentClient.GetCommunicator().GetUserStats(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));

            Stats.Content = parse(stats);
        }

        private string parse(string stats)
        {
            const int bracketMinCount = 2;
            string ret = "";
            int bracketCount = 0;
            int firstIndexToRemove = 0;

            while (stats[firstIndexToRemove] != '\0')
            {
                firstIndexToRemove++;
            }

            for(int i = 0; i < stats.Length; i++)
            {
                if (stats[i].Equals('{')) bracketCount++;
                else if (stats[i].Equals('}')) bracketCount--;

                if(bracketCount >= bracketMinCount && !stats[i].Equals('"') && !stats[i].Equals('{') && ((stats[i - 1].Equals(':') && stats[i].Equals(' ')) || !stats[i].Equals(' ')))
                {
                    if (stats[i].Equals(',')) ret += '\n';
                    else ret += stats[i];
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
