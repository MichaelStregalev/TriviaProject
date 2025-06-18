using BackendTrivia;
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
using System.Windows.Threading;

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

        // <-- MENU CONTROLLER -->
        private BackendTrivia.Menu menuController;

        // <-- TIMER -->
        private DispatcherTimer refreshTimer;

        // <-- ROOM, WILL HELP US WITH REFRESHING -->
        private BackendTrivia.Room roomBackend;

        public JoinRoomPage(String username, BackendTrivia.Menu menuController)
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
            // Menu controller
            this.menuController = menuController;
            // Room
            this.roomBackend = new BackendTrivia.Room(menuController.GetCommunicator());
            
            // Create timer
            refreshTimer = new DispatcherTimer();
            refreshTimer.Interval = TimeSpan.FromSeconds(3);
            refreshTimer.Tick += (sender, e) => LoadRooms();

            // Start the timer
            refreshTimer.Start();

            // Optionally, you can hook to Unloaded event to stop timer when leaving the page
            this.Unloaded += (s, e) => refreshTimer.Stop();
        }

        private void LoadRooms()
        {
            try
            {
                RoomListPanel.Children.Clear();

                List<Responses.RoomData> rooms = roomBackend.GetRooms();

                if (rooms.Count == 0)
                {
                    RoomListPanel.Children.Add(new TextBlock
                    {
                        Text = "No rooms Available to join right now!",
                        FontSize = 20,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                        HorizontalAlignment = HorizontalAlignment.Center,
                        Margin = new Thickness(0, 50, 0, 0)
                    });

                    return;
                }
                
                foreach (var room in rooms)
                {
                    StackPanel roomRow = new StackPanel
                    {
                        Orientation = Orientation.Horizontal,
                        Margin = new Thickness(10),
                        HorizontalAlignment = HorizontalAlignment.Stretch
                    };

                    // Room name
                    TextBlock nameBlock = new TextBlock
                    {
                        Text = room.Name,
                        Width = 150,
                        Foreground = (Brush)FindResource("HotPink"),
                        FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                        FontSize = 20,
                    };

                    // Room ID
                    TextBlock idBlock = new TextBlock
                    {
                        Text = $"ID: {room.Id}",
                        Width = 80,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        FontSize = 16,
                        Margin = new Thickness(0, 0, 20, 0)
                    };

                    // Get the amount of current players in the room
                    int currentPlayers = roomBackend.GetPlayersInRoom(room.Id).Count;

                    // Players count
                    TextBlock playersCountBlock = new TextBlock
                    {
                        Width = 140,
                        Margin = new Thickness(0, 0, 40, 0)
                    };

                    playersCountBlock.Inlines.Add(new Run($"{currentPlayers} / ")
                    {
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        FontSize = 16,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                    });

                    playersCountBlock.Inlines.Add(new Run($"{room.MaxPlayers}")
                    {
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        FontSize = 16,
                        Foreground = (Brush)FindResource("MidnightPurple"),
                    });

                    // Join Button
                    Button joinButton = new Button
                    {
                        Content = "Join",
                        Tag = new { Id = room.Id, Name = room.Name },
                        Width = 90,
                        Height = 30,
                        Margin = new Thickness(80, 0, 0, 0),
                        Style = (Style)FindResource("RoundedButtonStyle"),
                        FontSize = 14
                    };
                    joinButton.Click += JoinButton_Click;
                    joinButton.MouseEnter += Button_MouseEnter;
                    joinButton.MouseLeave += Button_MouseLeave;

                    roomRow.Children.Add(nameBlock);
                    roomRow.Children.Add(idBlock);
                    roomRow.Children.Add(playersCountBlock);
                    roomRow.Children.Add(joinButton);

                    RoomListPanel.Children.Add(roomRow);
                }
            }
            catch (Exception ex)
            {
                new StyledMessageBox("Failed to load rooms: " + ex.Message).Show();
            }
        }
        private void JoinButton_Click(object sender, RoutedEventArgs e)
        {
            Button joinButton = sender as Button;
            if (joinButton == null)
            {
                return;
            }

            // Get the info of the room
            dynamic tag = joinButton.Tag;
            uint roomId = tag.Id;
            string roomName = tag.Name;

            try
            {
                // Let the Menu object handle the request
                Room joinedRoom = menuController.JoinRoom(roomId);

                NavigationService.Navigate(new RoomPage(roomId, username, roomName, false, joinedRoom));
            }
            catch (Exception ex)
            {
                new StyledMessageBox("Failed to join room.").Show();
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
    }
}