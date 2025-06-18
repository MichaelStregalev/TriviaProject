using BackendTrivia;
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

        // <-- The current question count -->
        private uint currentQuestionNumber;
        private uint amountOfQuestions;

        // <-- GAME CONTROLLER -->
        private BackendTrivia.Game gameController;

        // <-- TIMER DISPATCHER -->
        private DispatcherTimer answerTimer;

        // <-- DATE VARIABLE, WILL BE USED TO GET ANSWERTIME OF QUESTION -->
        private DateTime questionStartTime;


        public GamePage(string username, uint answerTime, uint amountOfQuestions, BackendTrivia.Game gameController)
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

            this.currentQuestionNumber = 1;             // The current question the user is on
            this.amountOfQuestions = amountOfQuestions; // The amount of questions in the game

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

            // Dynamically adjust font size based on question length
            int length = question.Question.Length;

            if (length > 50)
                QuestionText.FontSize = 24;
            else if (length > 30)
                QuestionText.FontSize = 28;
            else if (length > 20)
                QuestionText.FontSize = 32;
            else if (length > 10)
                QuestionText.FontSize = 38;
            else
                QuestionText.FontSize = 46;

            // Put the radio buttons in a list for easier access
            var answerButtons = new List<RadioButton> { AnswerRadio1, AnswerRadio2, AnswerRadio3, AnswerRadio4 };

            // Loop through and assign available answers
            for (int i = 0; i < answerButtons.Count; i++)
            {
                if (i < question.Answers.Count)
                {
                    // 1-based index in question.Answers
                    answerButtons[i].Content = question.Answers[(uint)i + 1];
                    answerButtons[i].Tag = (uint)(i + 1);
                    answerButtons[i].Visibility = Visibility.Visible;
                    answerButtons[i].IsEnabled = true;

                    // Adjust font size dynamically based on answer length
                    if (question.Answers[(uint)i + 1].Length > 30)
                        answerButtons[i].FontSize = 16;
                    else if (question.Answers[(uint)i + 1].Length > 20)
                        answerButtons[i].FontSize = 20;
                    else if (question.Answers[(uint)i + 1].Length > 10)
                        answerButtons[i].FontSize = 26;
                    else
                        answerButtons[i].FontSize = 30; // default
                }
                else
                {
                    answerButtons[i].Content = null;
                    answerButtons[i].Visibility = Visibility.Collapsed;
                    answerButtons[i].IsEnabled = false;
                }

                answerButtons[i].IsChecked = false;
            }

            // Update the question progression text
            QuestionProgressionText.Text = $"{currentQuestionNumber} / {amountOfQuestions}";

            timeLeft = this.answerTimeout;
            TimerText.Text = timeLeft.ToString();
            questionStartTime = DateTime.Now;
            answerTimer.Start();
        }
        private void SubmitAnswer(uint answerId, double answerTime)
        {
            var response = gameController.SubmitAnswer(answerId, answerTime);

            this.currentQuestionNumber++; // Symbolize getting the next question..

            if (response.CorrectAnswerId == answerId)
            {
                new StyledMessageBox("Correct answer!").Show();
            }
            else if (answerId == 0)
            {
                new StyledMessageBox("Didn't answer!").Show();
            }
            else
            {
                new StyledMessageBox("Wrong answer!").Show();
            }

            if (response.Status == (uint)Codes.GameCodes.MORE_QUESTIONS)
            {
                LoadNextQuestion();
            }
            else if (response.Status == (uint)Codes.GameCodes.NO_MORE_QUESTIONS)
            {
                answerTimer.Stop();
                NavigationService.Navigate(new GameResultsPage(username, gameController));
            }
        }
        private void SubmitButton_Click(object sender, RoutedEventArgs e)
        {
            uint selectedAnswerId = GetSelectedAnswerId();

            if (selectedAnswerId == 0)
            {
                new StyledMessageBox("Please select an answer before submitting.").Show();
                return;
            }

            // Stop the timer
            answerTimer.Stop();

            // Calculate time taken to answer
            double timeTaken = (DateTime.Now - questionStartTime).TotalSeconds;

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
                new StyledMessageBox("An error occurred while trying to leave the game: " + ex.Message).Show();
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

        private void Page_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                uint selectedAnswerId = GetSelectedAnswerId();

                if (selectedAnswerId == 0)
                {
                    new StyledMessageBox("Please select an answer before submitting.").Show();
                    return;
                }

                // Stop the timer
                answerTimer.Stop();

                // Calculate time taken to answer
                double timeTaken = (DateTime.Now - questionStartTime).TotalSeconds;

                // Submit the answer
                SubmitAnswer(selectedAnswerId, timeTaken);
            }
        }
    }
}
