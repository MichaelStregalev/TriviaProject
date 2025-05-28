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
    public partial class StatisticsPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor selectCursor;
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- The username of the User -->
        private String username;
        public StatisticsPage(String username)
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

            UsernameTextBlock.Text = username;

            LoadStats();
        }

        private void LoadStats()
        {

        }

        private void HighScores_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new HighScoresPage(this.username));
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new MenuPage(this.username));
        }

        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void Grid_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Grid)sender).Cursor = selectCursor;
        }

        private void Grid_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Grid)sender).Cursor = pointerCursor;
        }
    }
}
