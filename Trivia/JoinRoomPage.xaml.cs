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
    public partial class JoinRoomPage : Page
    {
        // <-- Custom Cursors -->
        private Cursor selectCursor;
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- The username of the User -->
        private String username;

        public JoinRoomPage(String username)
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

            LoadRooms();
        }
        private void LoadRooms()
        {
            //TODO: var rooms = GetAvailableRooms();

            RoomListPanel.Children.Clear();

            //if (rooms.Count == 0)
            //{
            //    RoomListPanel.Children.Add(new TextBlock
            //    {
            //        Text = "No rooms Available to join right now!",
            //        FontSize = 20,
            //        Foreground = (Brush)FindResource("MidnightPurple"),
            //        FontFamily = new FontFamily("pack://application:,,,/Trivia;component/Fonts/#Anomalia v2 AAA Medium"),
            //        HorizontalAlignment = HorizontalAlignment.Center,
            //        Margin = new Thickness(0, 50, 0, 0)
            //    });
            //}


        }
        private void Button_MouseEnter(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = questionMarkCursor;
        }

        private void Button_MouseLeave(object sender, MouseEventArgs e)
        {
            ((Button)sender).Cursor = pointerCursor;
        }

        private void ScrollViewer_MouseEnter(object sender, MouseEventArgs e)
        {
            ((ScrollViewer)sender).Cursor = selectCursor;
        }

        private void ScrollViewer_MouseLeave(object sender, MouseEventArgs e)
        {
            ((ScrollViewer)sender).Cursor = pointerCursor;
        }
        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.GoBack();
        }
        private void JoinButton_Click(object sender, RoutedEventArgs e)
        {
            string roomName = (string)((Button)sender).Tag;
            MessageBox.Show($"Joining room: {roomName}");
            // TODO: Navigate to the room
        }
    }
}