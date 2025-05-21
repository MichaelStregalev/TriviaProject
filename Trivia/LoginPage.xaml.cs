using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Trivia
{
    /// <summary>
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : Page
    {
        private Frame mainFrame;

        public LoginPage(Frame frame)
        {
            InitializeComponent();
            mainFrame = frame;
        }

        private void Input_TextChanged(object sender, RoutedEventArgs e)
        {
            ValidateInputs();
        }

        private void ValidateInputs()
        {
            ValidateInput(UsernameInput.Text, UsernameValidIcon, UsernameInvalidIcon);
            ValidateInput(!string.IsNullOrEmpty(PasswordInput.Password), PasswordValidIcon, PasswordInvalidIcon);
        }

        private void ValidateInput(string text, Image validIcon, Image invalidIcon)
        {
            if (!string.IsNullOrWhiteSpace(text))
            {
                validIcon.Visibility = Visibility.Visible;
                invalidIcon.Visibility = Visibility.Collapsed;
            }
            else
            {
                validIcon.Visibility = Visibility.Collapsed;
                invalidIcon.Visibility = Visibility.Visible;
            }
        }

        private void ValidateInput(bool isValid, Image validIcon, Image invalidIcon)
        {
            if (isValid)
            {
                validIcon.Visibility = Visibility.Visible;
                invalidIcon.Visibility = Visibility.Collapsed;
            }
            else
            {
                validIcon.Visibility = Visibility.Collapsed;
                invalidIcon.Visibility = Visibility.Visible;
            }
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(UsernameInput.Text) && !string.IsNullOrEmpty(PasswordInput.Password))
            {
                // For now, proceed to menu
                mainFrame.Navigate(new MenuPage(mainFrame));
            }
            else
            {
                MessageBox.Show("Please fill in all fields correctly.");
            }
        }
    }

}
