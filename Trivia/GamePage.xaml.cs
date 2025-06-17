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
using System.Windows.Threading;

namespace Trivia
{
    public partial class GamePage : Page
    {
        // <-- Custom Cursors -->
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- The username of the User -->
        private String username;

        // <-- The answer timeout for each question -->
        private uint answerTimeout;
        private uint timeLeft;

        // <-- GAME CONTROLLER -->
        private BackendTrivia.Game gameController;

        // <-- TIMER DISPATCHER -->
        private DispatcherTimer answerTimer;

        // <-- DATE VARIABLE, WILL BE USED TO GET ANSWERTIME OF QUESTION -->
        private DateTime questionStartTime;


        public GamePage(string username, uint answerTime, BackendTrivia.Game gameController)
        {
            InitializeComponent();

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;

            this.username = username;                    // Username of user
            this.answerTimeout = answerTime;             // Answer timeout
            this.timeLeft = answerTime;                  // Time left for user to answer

            // The game controller
            this.gameController = gameController;

            // Setup answerTimer!
            answerTimer = new DispatcherTimer();
            answerTimer.Interval = TimeSpan.FromSeconds(1);
            answerTimer.Tick += (sender, e) => AnswerTimer_Tick();

            LoadNextQuestion();
        }
        private void AnswerTimer_Tick()
        {
            this.timeLeft--;
            TimerText.Text = this.timeLeft.ToString();

            if (timeLeft <= 0)
            {
                answerTimer.Stop();
                SubmitAnswer(0, this.answerTimeout);     // ID 0 means invalid/no answer
            }
        }
        private void LoadNextQuestion()
        {
            var question = gameController.GetQuestion();
            QuestionText.Text = question.Question;

            // Put the radio buttons in a list for easier access
            var answerButtons = new List<RadioButton> { AnswerRadio1, AnswerRadio2, AnswerRadio3, AnswerRadio4 };

            // Loop through and assign available answers
            for (int i = 0; i < answerButtons.Count; i++)
            {
                if (i < question.Answers.Count)
                {
                    answerButtons[i].Content = question.Answers[(uint)i];
                    answerButtons[i].Tag = (i + 1).ToString();              // ID: 1-based index
                    answerButtons[i].Visibility = Visibility.Visible;
                    answerButtons[i].IsEnabled = true;
                }
                else
                {
                    answerButtons[i].Content = null;
                    answerButtons[i].Visibility = Visibility.Collapsed;
                    answerButtons[i].IsEnabled = false;
                }

                answerButtons[i].IsChecked = false;
            }

            timeLeft = this.answerTimeout;
            TimerText.Text = timeLeft.ToString();
            questionStartTime = DateTime.Now;
            answerTimer.Start();
        }
        private void SubmitAnswer(uint answerId, double answerTime)
        {
            var response = gameController.SubmitAnswer(answerId, answerTime);

            if (response.Status == (uint)Codes.GameCodes.MORE_QUESTIONS)
            {
                LoadNextQuestion();
            }
            else if (response.Status == (uint)Codes.GameCodes.NO_MORE_QUESTIONS)
            {
                //NavigationService.Navigate(new GameResultsPage(username, gameController));
            }
        }
        private void SubmitButton_Click(object sender, RoutedEventArgs e)
        {
            uint selectedAnswerId = GetSelectedAnswerId();

            if (selectedAnswerId == 0)
            {
                MessageBox.Show("Please select an answer before submitting.");
                return;
            }

            // Stop the timer
            answerTimer.Stop();

            // Calculate time taken to answer
            double timeTaken = (uint)(DateTime.Now - questionStartTime).TotalSeconds;

            // Submit the answer
            SubmitAnswer(selectedAnswerId, timeTaken);
        }
        private uint GetSelectedAnswerId()
        {
            if (AnswerRadio1.IsChecked == true) return (uint)AnswerRadio1.Tag;
            if (AnswerRadio2.IsChecked == true) return (uint)AnswerRadio2.Tag;
            if (AnswerRadio3.IsChecked == true) return (uint)AnswerRadio3.Tag;
            if (AnswerRadio4.IsChecked == true) return (uint)AnswerRadio4.Tag;
            return 0;   // 0 means no answer selected (invalid ID)
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            // Stop the timer
            answerTimer.Stop();

            try
            {
                // Inform the server the player is leaving
                gameController.LeaveGame();
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred while trying to leave the game: " + ex.Message);
            }

            // Navigate back to menu (replace MenuPage with your actual menu page)
            NavigationService.Navigate(new MenuPage(username, new BackendTrivia.Menu(gameController.GetCommunicator())));
        }
        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void RadioButton_MouseEnter(object sender, MouseEventArgs e)
        {
            ((RadioButton)sender).Cursor = questionMarkCursor;
        }

        private void RadioButton_MouseLeave(object sender, MouseEventArgs e)
        {
            ((RadioButton)sender).Cursor = pointerCursor;
        }
    }
}
