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
    /// Interaction logic for RoomPage.xaml
    /// </summary>
    public partial class RoomPage : Page
    {
        public RoomPage(Frame frame, string roomName, string maxPlayers, string questionCount, string answerTimeout)
        {
            InitializeComponent();

            RoomNameText.Text = $"Name: {roomName}";
            MaxPlayersText.Text = $"Max players: {maxPlayers}";
            QuestionCountText.Text = $"Question count: {questionCount}";
            AnswerTimeoutText.Text = $"Answer timeout: {answerTimeout}";
        }
    }
}
