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
using Xceed.Wpf.Toolkit;

namespace Trivia
{
    /// <summary>
    /// Interaction logic for CreateRoomPage.xaml
    /// </summary>
    public partial class CreateRoomPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor selectCursor;            // Selecting cursor - for input
        private Cursor questionMarkCursor;      // Question mark cursor
        private Cursor pointerCursor;           // Pointer cursor - default

        // <-- Username of the user -->
        private string username;
        public CreateRoomPage(string username)
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
        }
        private void ButtonClicked(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn)
            {
                switch (btn.Name)
                {
                    case "CreateButton":
                        // Validation checks
                        NavigationService.Navigate(new RoomPage(this.username, RoomNameInput.Text, true));
                        break;

                    case "ExitButton":
                        NavigationService.GoBack();
                        break;

                    default:
                        // Nothing happens
                        break;
                }
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

        private void TextBox_MouseEnter(object sender, MouseEventArgs e)
        {
            ((TextBox)sender).Cursor = selectCursor;
        }

        private void TextBox_MouseLeave(object sender, MouseEventArgs e)
        {
            ((TextBox)sender).Cursor = pointerCursor;
        }

        private void IntegerUpDown_MouseEnter(object sender, MouseEventArgs e)
        {
            ((IntegerUpDown)sender).Cursor = selectCursor;
        }

        private void IntegerUpDown_MouseLeave(object sender, MouseEventArgs e)
        {
            ((IntegerUpDown)sender).Cursor = pointerCursor;
        }

        private void IncreaseButton_Click(object sender, RoutedEventArgs e)
        {
            IntegerUpDownHelpers.IncreaseButton_Click(sender, e);
        }

        private void DecreaseButton_Click(object sender, RoutedEventArgs e)
        {
            IntegerUpDownHelpers.DecreaseButton_Click(sender, e);
        }

        private void Page_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                //Validation checks
                NavigationService.Navigate(new RoomPage(this.username, RoomNameInput.Text, true));
            }
        }
    }
}
