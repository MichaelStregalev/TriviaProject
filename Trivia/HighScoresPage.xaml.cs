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

namespace Trivia
{
    public partial class HighScoresPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor selectCursor;
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- The username of the User -->
        private String username;

        // <-- MENU CONTROLLER -->
        private BackendTrivia.Menu menuController;
        public HighScoresPage(String username, BackendTrivia.Menu menuController)
        {
            InitializeComponent();

            this.username = username;

            selectCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Select.cur")).Stream);

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;
            // Menu controller
            this.menuController = menuController;

            DisplayHighScores();
        }

        private void DisplayHighScores()
        {
            List<(string, int)> scores;

            try
            {
                scores = menuController.HighScores();
            }
            catch
            {
                new StyledMessageBox("Failed to load high scores.").Show();
                return;
            }

            LeaderboardStack.Children.Clear();

            for (int i = 0; i < Math.Min(5, scores.Count); i++)
            {
                var (name, score) = scores[i];

                StackPanel row = new StackPanel
                {
                    Orientation = Orientation.Horizontal,
                    Margin = new Thickness(0, 5, 0, -1)
                };

                TextBlock numberAndName = new TextBlock
                {
                    Text = $"{i + 1}.   {name}",
                    FontSize = 24,
                    FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                    Foreground = (Brush)FindResource("MidnightPurple"),
                    Width = 600,
                    HorizontalAlignment = HorizontalAlignment.Left
                };

                TextBlock scoreText = new TextBlock
                {
                    Text = score.ToString(),
                    FontSize = 24,
                    FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                    Foreground = (Brush)FindResource("MidnightPurple"),
                    HorizontalAlignment = HorizontalAlignment.Right
                };

                // If one of the highscores is the current user...
                if (name.Equals(this.username))
                {
                    numberAndName.FontSize += 4;
                    numberAndName.Foreground = (Brush)FindResource("HotPink");
                    scoreText.FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont");
                    scoreText.Foreground = (Brush)FindResource("HotPink");
                    scoreText.FontSize += 4;
                }

                row.Children.Add(numberAndName);
                row.Children.Add(scoreText);

                LeaderboardStack.Children.Add(row);
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
            NavigationService.Navigate(new MenuPage(this.username, this.menuController));
        }
    }
}