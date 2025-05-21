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
    /// Interaction logic for CreateRoomPage.xaml
    /// </summary>
    public partial class CreateRoomPage : Page
    {
        private Frame mainFrame;

        public CreateRoomPage(Frame frame)
        {
            InitializeComponent();
            mainFrame = frame;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            // TODO: validate inputs

            // Navigate to room page with parameters - for now, hardcoded
            mainFrame.Navigate(new RoomPage(mainFrame, RoomNameInput.Text, MaxPlayersInput.Text, QuestionCountInput.Text, AnswerTimeoutInput.Text));
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            mainFrame.Navigate(new MenuPage(mainFrame));
        }
    }

}
