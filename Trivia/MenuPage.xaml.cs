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
    public partial class MenuPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor questionMarkCursor;      // Question mark cursor
        private Cursor pointerCursor;           // Pointer cursor - default

        // <-- Username of the user -->
        private string username;

        // <-- COMMUNICATOR -->
        private BackendTrivia.Menu menuController;
        public MenuPage(string username, BackendTrivia.Menu menuController)
        {
            InitializeComponent();

            this.username = username;
            UsernameBlock.Text = "Welcome, " + username;

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;

            this.menuController = menuController;
        }

        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void ButtonClicked(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn)
            {
                switch (btn.Name)
                {
                    case "CreateRoomButton":
                        NavigationService.Navigate(new CreateRoomPage(this.username, this.menuController));
                        break;

                    case "JoinRoomButton":
                        NavigationService.Navigate(new JoinRoomPage(this.username, this.menuController));
                        break;

                    case "StatisticsButton":
                        NavigationService.Navigate(new StatisticsPage(this.username, this.menuController));
                        break;

                    case "ExitButton":
                        NavigationService.Navigate(new WelcomePage());
                        break;

                    default:
                        // Nothing happens
                        break;
                }
            }
        }
    }
}
