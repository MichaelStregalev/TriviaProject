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
    public partial class LoginPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor SelectCursor;
        private Cursor questionMarkCursor;      // Question mark cursor
        private Cursor pointerCursor;           // Pointer cursor - default

        // <-- Validity Icons -->
        private BitmapImage valid;
        private BitmapImage invalid;
        public LoginPage()
        {
            InitializeComponent();

            valid = new BitmapImage(new Uri("pack://application:,,,/Images/valid.png"));
            invalid = new BitmapImage(new Uri("pack://application:,,,/Images/invalid.png"));

            SelectCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Select.cur")).Stream);

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;

            this.Focus();
        }

        private void TextBox_MouseEnter(object sender, MouseEventArgs e)
        {
            ((TextBox)sender).Cursor = SelectCursor;
        }

        private void TextBox_MouseLeave(object sender, MouseEventArgs e)
        {
            ((TextBox)sender).Cursor = pointerCursor;
        }

        private void PasswordBox_MouseEnter(object sender, MouseEventArgs e)
        {
            ((PasswordBox)sender).Cursor = SelectCursor;
        }

        private void PasswordBox_MouseLeave(object sender, MouseEventArgs e)
        {
            ((PasswordBox)sender).Cursor = pointerCursor;
        }

        private void ExitButton_MouseEnter(object sender, MouseEventArgs e)
        {
            ExitButton.Cursor = questionMarkCursor;
        }

        private void ExitButton_MouseLeave(object sender, MouseEventArgs e)
        {
            ExitButton.Cursor = pointerCursor;
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.GoBack();
        }

        private void Page_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                //ValidateFields();
                MessageBox.Show("Login successful!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
                NavigationService.Navigate(new MenuPage(UsernameInput.Text));
            }
        }
    }
}
