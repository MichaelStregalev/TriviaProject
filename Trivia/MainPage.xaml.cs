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
    /// <summary>
    /// Interaction logic for MainPage.xaml
    /// </summary>
    public partial class MainPage : Page
    {
        private Frame mainFrame;

        public MainPage(Frame frame)
        {
            InitializeComponent();
            mainFrame = frame;
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new LoginPage(mainFrame));
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new SignupPage(mainFrame));
        }
    }
}
