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
    public partial class WelcomePage : Page
    {
        // <-- Custom Cursors -->
        private Cursor magnifyingCursor;
        private Cursor questionMarkCursor;      // Question mark cursor
        private Cursor pointerCursor;           // Pointer cursor - default

        public WelcomePage()
        {
            InitializeComponent();

            magnifyingCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Magnifying.cur")).Stream);

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;
        }

        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void NewHereInfoBlock_MouseEnter(object sender, MouseEventArgs e)
        {
            NewHereInfoBlock.Cursor = magnifyingCursor;
        }

        private void NewHereInfoBlock_MouseLeave(object sender, MouseEventArgs e)
        {
            NewHereInfoBlock.Cursor = pointerCursor;
        }

        private void NavigateToPage(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;

            if (btn.Name == "SignupButton")
            {
                NavigationService.Navigate(new SignupPage());
            }
            else if (btn.Name == "LoginButton")
            {
                NavigationService.Navigate(new LoginPage());
            }
        }
    }
}