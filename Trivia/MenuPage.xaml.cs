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
    /// Interaction logic for MenuPage.xaml
    /// </summary>
    public partial class MenuPage : Page
    {
        private Frame mainFrame;

        public MenuPage(Frame frame)
        {
            InitializeComponent();
            mainFrame = frame;
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new JoinRoomPage(mainFrame));
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new CreateRoomPage(mainFrame));
        }

        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new Statistics(mainFrame));
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new MainPage(mainFrame));
        }
    }

}
