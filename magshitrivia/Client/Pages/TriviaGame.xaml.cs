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

    public partial class TriviaGame : Window
    {
        public System.Diagnostics.Stopwatch watch;
        public string lastClickedAnswer;
        public JsonQuestionAnswer qa;
        public User currClient;
        public TriviaGame(User client)
        {
            currClient = new User(client);
            InitializeComponent();
            initializeQA();
        }
        public void initializeQA()
        {
            JsonGlobalRequest request = new JsonGlobalRequest
            {
                type = "get_question"
            };
            qa = currClient.GetCommunicator().GetQuestionAnswer(Newtonsoft.Json.JsonConvert.SerializeObject(request, new Newtonsoft.Json.JsonSerializerSettings
            {
                NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                Formatting = Newtonsoft.Json.Formatting.None
            }));
            watch = System.Diagnostics.Stopwatch.StartNew();
            if (qa.Data == null)
            {
                JsonGlobalRequest jsonRequest = new JsonGlobalRequest
                {
                    type = "get_game_results"
                };
                string gameResult = currClient.GetCommunicator().GetGameResults(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
                {
                    NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                    Formatting = Newtonsoft.Json.Formatting.None
                }));
                GameScore gs = new GameScore(gameResult, currClient);
                gs.Show();
                Close();
            }

            if (qa.Data != null)
            {
                QuestionBox.Text = qa.Data.Question;
                Answer.Items.Clear();
                foreach (KeyValuePair<string, string> entry in qa.Data.Answers)
                {
                    Answer.Items.Add(entry.Value);
                }
            }
        }
        private void Answer_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                lastClickedAnswer = Answer.Items[Answer.SelectedIndex].ToString();
                SubmitAnswer();
                initializeQA();
            }
            catch
            {

            }
        }

        public void SubmitAnswer()
        {
            watch.Stop();
            foreach (KeyValuePair<string, string> entry in qa.Data.Answers)
            {
                if (entry.Value.Equals(lastClickedAnswer))
                {

                    JsonSubmitAnswerRequest jsonRequest = new JsonSubmitAnswerRequest
                    {
                        type = "submit_answer",
                        answer_id = int.Parse(entry.Key),
                        time_to_answer = watch.ElapsedMilliseconds
                    };
                    string resp = currClient.GetCommunicator().SubmitAnswerRequest(Newtonsoft.Json.JsonConvert.SerializeObject(jsonRequest, new Newtonsoft.Json.JsonSerializerSettings
                    {
                        NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore,
                        Formatting = Newtonsoft.Json.Formatting.None
                    }));
                }
            }
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
            }
        }
    }
}
