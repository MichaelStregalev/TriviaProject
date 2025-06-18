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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Trivia
{
    public partial class StyledMessageBox : Window
    {
        public StyledMessageBox(string message)
        {
            InitializeComponent();
            MessageText.Text = message;

            // Set font size based on message length
            if (message.Length > 50)
                MessageText.FontSize = 12;
            else if (message.Length > 30)
                MessageText.FontSize = 16;
            else if (message.Length > 20)
                MessageText.FontSize = 20;
            else
                MessageText.FontSize = 25;

            // Start fully transparent
            this.Opacity = 0;

            // Fade In
            var fadeIn = new DoubleAnimation
            {
                From = 0,
                To = 1,
                Duration = TimeSpan.FromMilliseconds(500)
            };
            this.BeginAnimation(Window.OpacityProperty, fadeIn);

            // Schedule Fade Out after 1.5s
            var timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1.5)
            };
            timer.Tick += (s, e) =>
            {
                timer.Stop();

                // Fade Out
                var fadeOut = new DoubleAnimation
                {
                    From = 1,
                    To = 0,
                    Duration = TimeSpan.FromMilliseconds(500)
                };

                fadeOut.Completed += (s, e) => this.Close();
                this.BeginAnimation(Window.OpacityProperty, fadeOut);
            };
            timer.Start();
        }
        public static void Show(string message, Window owner = null)
        {
            var box = new StyledMessageBox(message);
            if (owner != null)
            {
                box.Owner = owner;
            }

            box.Show();

            var timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1.5)
            };

            timer.Tick += (s, e) =>
            {
                timer.Stop();
                box.Close();
            };

            timer.Start();
        }
        private void Close_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
