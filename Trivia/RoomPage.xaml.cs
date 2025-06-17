using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
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
    public partial class RoomPage : Page
    {
        // <-- The info of the room & user -->
        private uint roomId;
        private string username;
        private string roomName;
        private bool isAdmin;

        // <-- Custom Cursors -->
        private Cursor selectCursor;
        private Cursor pointerCursor;
        private Cursor questionMarkCursor;

        // <-- ROOM CONTROLLER -->
        private BackendTrivia.Room roomController;

        // <-- TIMERS FOR THREADS -->
        private DispatcherTimer playersUpdateTimer;
        private DispatcherTimer roomStateCheckTimer;

        public RoomPage(uint roomId, string username, string roomName, bool isAdmin, BackendTrivia.Room roomController)
        {
            InitializeComponent();

            this.roomId = roomId;
            this.username = username;
            this.roomName = roomName;
            this.isAdmin = isAdmin;

            RoomNameTextBlock.Text = roomName;

            selectCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Select.cur")).Stream);

            questionMarkCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/QuestionMark.cur")).Stream);

            pointerCursor = new Cursor(Application.GetResourceStream(
                new Uri("pack://application:,,,/Cursors/Pointer.cur")).Stream);

            // Default cursor - the pointer cursor
            this.Cursor = pointerCursor;
            // The room controller
            this.roomController = roomController;

            // Adjust GUI for admin
            if (isAdmin)
            {
                ExitOrCloseButton.Content = "Close Room";
                ExitOrCloseButton.Width = 200;
                StartGameButton.Visibility = Visibility.Visible;
                StartGameButton.Width = 200;
            }

            LoadInfo();

            SetupTimers();

            this.Unloaded += (s, e) => playersUpdateTimer?.Stop();
            this.Unloaded += (s, e) => roomStateCheckTimer?.Stop();
        }
        private void LoadInfo()
        {
            RoomInfoPanel.Children.Clear();

            try
            {
                var roomState = roomController.GetRoomState();
                var questionCount = roomState.AnswerCount;
                var answerTimeout = roomState.AnswerTimeOut;

                RoomInfoPanel.Children.Add(CreateInfoBlock($"Room ID: {roomId}"));
                RoomInfoPanel.Children.Add(CreateInfoBlock($"Questions: {questionCount}"));
                RoomInfoPanel.Children.Add(CreateInfoBlock($"Timeout: {answerTimeout}s"));
            }
            catch (Exception)
            {
                RoomInfoPanel.Children.Add(new TextBlock
                {
                    Text = "Error loading room info.",
                    FontSize = 14,
                    Foreground = (Brush)FindResource("HotPink")
                });
            }
        }

        // Helper method for creating an info block in LoadInfo
        TextBlock CreateInfoBlock(string text)
        {
            return new TextBlock
            {
                Text = text,
                FontSize = 16,
                FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                Foreground = (Brush)FindResource("MidnightPurple"),
                Margin = new Thickness(10, 0, 10, 0),
                VerticalAlignment = VerticalAlignment.Center
            };
        }
        private void SetupTimers()
        {
            // Timer for updating players list
            playersUpdateTimer = new DispatcherTimer();
            playersUpdateTimer.Interval = TimeSpan.FromSeconds(3);
            playersUpdateTimer.Tick += (sender, e) => LoadPlayers();
            playersUpdateTimer.Start();


            // Timer for checking room state
            roomStateCheckTimer = new DispatcherTimer();
            roomStateCheckTimer.Interval = TimeSpan.FromSeconds(3);
            roomStateCheckTimer.Tick += (sender, e) => CheckRoomState();
            roomStateCheckTimer.Start();
        }
        private void CheckRoomState()
        {
            // This refresh will check one of the following - if the game still exists (has not been closed) - and if it does,
            // did the game start already? if so - start the game!
            // if the game does not exist - leave it...

            try
            {
                var state = roomController.GetRoomState();

                if (state.HasGameBegun)
                {
                    // Navigate to game page
                    NavigationService.Navigate(new GamePage(username, state.AnswerTimeOut, new BackendTrivia.Game(roomController.GetCommunicator())));
                }
            }
            catch
            {
                // Stop updating the players if the player is going to leave the room.
                // Preventing an error from occurring in the server.
                playersUpdateTimer.Stop();

                // Leave the room!
                roomController.LeaveRoom();
                NavigationService.Navigate(new MenuPage(username, new BackendTrivia.Menu(roomController.GetCommunicator())));
            }
        }
        private void LoadPlayers()
        {
            PlayersListPanel.Children.Clear();

            List<string> players;
            try
            {
                players = roomController.GetPlayersInRoom();
            }
            catch (Exception)
            {
                return;
            }

            int playerCount = players.Count;

            // Calculate dynamic font size (example: between 14 and 20)
            // If fewer players, font size bigger; if many players, smaller font
            double fontSize = Codes.MAX_FONT_SIZE_PLAYER - (playerCount - 1) * Codes.MULTIPLICATION_FONT;

            // Showcase the current user

            TextBlock currentPlayerText = new TextBlock
            {
                Text = username,
                FontSize = fontSize + Codes.BIGGER_FONT_ADDITION,
                FontFamily = (FontFamily)FindResource("AnomaliaUltraBoldFont"),
                Foreground = (Brush)FindResource("HotPink"),
                Margin = new Thickness(5, 2, 5, 2)
            };

            PlayersListPanel.Children.Add(currentPlayerText);

            foreach (string player in players)
            {
                if (!player.Equals(username))
                {
                    TextBlock playerText = new TextBlock
                    {
                        Text = player,
                        FontSize = fontSize,
                        FontFamily = (FontFamily)FindResource("AnomaliaMediumFont"),
                        Foreground = (Brush)FindResource("MidnightPurple"),
                        Margin = new Thickness(5, 2, 5, 2)
                    };

                    PlayersListPanel.Children.Add(playerText);
                }
            }
        }
        private void ExitOrCloseButton_Click(object sender, RoutedEventArgs e)
        {
            if (isAdmin)
            {
                // Close the room and navigate to MenuPage
                try
                {
                    playersUpdateTimer.Stop();
                    roomStateCheckTimer.Stop();

                    LoadPlayers();      // Last time, we will try and laod the players

                    BackendTrivia.Menu nextController = roomController.CloseRoom();
                    NavigationService.Navigate(new MenuPage(username, nextController));
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to close the room: " + ex.Message);
                }
            }
            else
            {
                // Leave the room and navigate to MenuPage
                try
                {
                    playersUpdateTimer.Stop();
                    roomStateCheckTimer.Stop();

                    LoadPlayers();      // Last time, we will try and laod the players

                    BackendTrivia.Menu nextController = roomController.LeaveRoom();
                    NavigationService.Navigate(new MenuPage(username, nextController));
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to leave the room: " + ex.Message);
                }
            }
        }
        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                playersUpdateTimer.Stop();
                roomStateCheckTimer.Stop();

                LoadPlayers();      // Last time, we will try and laod the players

                roomController.StartRoom();
                // Get the state of the room
                var state = roomController.GetRoomState();

                // disable the start button after pressing - will prevent from pressing multiple times while the game is starting
                StartGameButton.IsEnabled = false;

                NavigationService.Navigate(new GamePage(username, state.AnswerTimeOut, new BackendTrivia.Game(roomController.GetCommunicator())));
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to start the game: " + ex.Message);
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
    }
}