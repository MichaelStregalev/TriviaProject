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
using static Trivia.Responses;

namespace Trivia
{
    public partial class GameResultsPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor selectCursor;
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- The username of the User -->
        private String username;

        // <-- GAME CONTROLLER -->
        private BackendTrivia.Game gameController;

        // <-- GAME RESULTS DISPATCHER -->
        private DispatcherTimer resultsRefresh;

        public GameResultsPage(String username, BackendTrivia.Game gameController)
        {
            InitializeComponent();

            selectCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Select.cur")).Stream);

            questionMarkCursor = new Cursor(Application.GetResourceStream(
               new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;

            this.username = username;               // The username of the user
            this.gameController = gameController;   // The Game controller

            // Setup answerTimer!
            resultsRefresh = new DispatcherTimer();
            resultsRefresh.Interval = TimeSpan.FromSeconds(3);
            resultsRefresh.Tick += (sender, e) => RefreshResults();

            resultsRefresh.Start();
        }
        private void RefreshResults()
        {
            try
            {
                // Clear old data
                ResultsStack.Children.Clear();

                List<PlayerResult> results = gameController.GetGameResults();

                // Stop the timer once results are available
                resultsRefresh.Stop();

                // Sort results
                var sortedResults = results
                    .OrderByDescending(r => r.CorrectAnswerCount + r.WrongAnswerCount)
                    .ThenByDescending(r => r.CorrectAnswerCount)
                    .ThenBy(r => r.AverageAnswerTime)
                    .ToList();

                for (int i = 0; i < sortedResults.Count; i++)
                {
                    var result = sortedResults[i];

                    StackPanel resultRow = new StackPanel
                    {
                        Orientation = Orientation.Horizontal,
                        Margin = new Thickness(10),
                        HorizontalAlignment = HorizontalAlignment.Stretch
                    };

                    // Rank
                    TextBlock rankBlock = new TextBlock
                    {
                        Text = $"{i + 1}.",
                        Width = 30,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                        FontSize = 20,
                        Margin = new Thickness(0, 0, 10, 0)
                    };

                    // Username
                    TextBlock usernameBlock = new TextBlock
                    {
                        Text = result.Username,
                        Width = 180,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                        FontSize = 20,
                        Margin = new Thickness(0, 0, 20, 0)
                    };

                    // Correct/Wrong
                    TextBlock statsBlock = new TextBlock
                    {
                        Text = $"✔ {result.CorrectAnswerCount}                      ✘ {result.WrongAnswerCount}",
                        Width = 300,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        FontSize = 20,
                        Margin = new Thickness(0, 0, 30, 0)
                    };

                    // Average Time
                    TextBlock timeBlock = new TextBlock
                    {
                        Text = $"⏱ {result.AverageAnswerTime:F2}s",
                        Width = 200,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        FontSize = 20
                    };

                    // If this is the current user..
                    if (result.Username.Equals(username))
                    {
                        rankBlock.Foreground = (Brush)FindResource("HotPink");
                        usernameBlock.Foreground = (Brush)FindResource("HotPink");
                        statsBlock.Foreground = (Brush)FindResource("HotPink");
                        timeBlock.Foreground = (Brush)FindResource("HotPink");
                    }

                    resultRow.Children.Add(rankBlock);
                    resultRow.Children.Add(usernameBlock);
                    resultRow.Children.Add(statsBlock);
                    resultRow.Children.Add(timeBlock);

                    ResultsStack.Children.Add(resultRow);
                }
            }
            catch (Exception)
            {
                // Game not finished yet
                ResultsStack.Children.Clear();

                ResultsStack.Children.Add(new TextBlock
                {
                    Text = "Game Results will appear when everybody has finished the game.",
                    FontSize = 20,
                    Foreground = (Brush)FindResource("MidnightPurple"),
                    FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                    TextWrapping = TextWrapping.Wrap,
                    TextAlignment = TextAlignment.Center,
                    HorizontalAlignment = HorizontalAlignment.Center,
                    Margin = new Thickness(0, 50, 0, 0)
                });
            }
        }
        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void ScrollViewer_MouseEnter(object sender, MouseEventArgs e)
        {
            ((ScrollViewer)sender).Cursor = selectCursor;
        }

        private void ScrollViewer_MouseLeave(object sender, MouseEventArgs e)
        {
            ((ScrollViewer)sender).Cursor = pointerCursor;
        }
        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            // Stop the refresh
            resultsRefresh.Stop();
            gameController.LeaveGame();

            NavigationService.Navigate(new MenuPage(this.username, new BackendTrivia.Menu(gameController.GetCommunicator())));
        }
    }
}
