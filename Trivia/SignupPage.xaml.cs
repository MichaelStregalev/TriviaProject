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
    public partial class SignupPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor SelectCursor;
        private Cursor questionMarkCursor;      // Question mark cursor
        private Cursor pointerCursor;           // Pointer cursor - default

        // <-- Validity Icons -->
        private BitmapImage valid;
        private BitmapImage invalid;

        // <-- LOGIN CONTROLLER -->
        private BackendTrivia.Login loginController;
        public SignupPage()
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

            this.loginController = new BackendTrivia.Login();

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

        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.GoBack();
        }

        private void SignupButton_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameInput.Text;
            string password = PasswordInput.Password;
            string email = EmailInput.Text;

            bool isUsernameValid = !string.IsNullOrWhiteSpace(username);
            bool isPasswordValid = !string.IsNullOrWhiteSpace(password);
            bool isEmailValid = !string.IsNullOrWhiteSpace(email);

            if (!isUsernameValid || !isPasswordValid || !isEmailValid)
            {
                ValidityMessageBlock.Text = "Please fill in all fields.";

                UsernameValidIcon.Source = invalid;
                PasswordValidIcon.Source = invalid;
                EmailValidIcon.Source = invalid;

                return;
            }

            try
            {
                BackendTrivia.Menu menuController = loginController.signup(username, password, email);
                NavigationService.Navigate(new MenuPage(username, menuController));
            }
            catch (Exception ex)
            {
                string message = ex.Message.ToLower();

                ValidityMessageBlock.Text = ex.Message;

                // Adjust icons based on error content
                UsernameValidIcon.Source = message.Contains("username") ? invalid : valid;
                PasswordValidIcon.Source = message.Contains("password") ? invalid : valid;
                EmailValidIcon.Source = message.Contains("email") ? invalid : valid;
            }
        }

        private void Page_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {

                string username = UsernameInput.Text;
                string password = PasswordInput.Password;
                string email = EmailInput.Text;

                bool isUsernameValid = !string.IsNullOrWhiteSpace(username);
                bool isPasswordValid = !string.IsNullOrWhiteSpace(password);
                bool isEmailValid = !string.IsNullOrWhiteSpace(email);

                if (!isUsernameValid || !isPasswordValid || !isEmailValid)
                {
                    ValidityMessageBlock.Text = "Please fill in all fields.";
                    
                    return;
                }

                try
                {
                    BackendTrivia.Menu menuController = loginController.signup(username, password, email);
                    NavigationService.Navigate(new MenuPage(username, menuController));
                }
                catch (Exception ex)
                {
                    string message = ex.Message.ToLower();

                    ValidityMessageBlock.Text = ex.Message;

                    // Adjust icons based on error content
                    UsernameValidIcon.Source = message.Contains("username") ? invalid : valid;
                    PasswordValidIcon.Source = message.Contains("password") ? invalid : valid;
                    EmailValidIcon.Source = message.Contains("email") ? invalid : valid;
                }
            }
        }
    }
}
