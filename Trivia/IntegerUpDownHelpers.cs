using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows;
using Xceed.Wpf.Toolkit;

namespace Trivia
{
    public static class IntegerUpDownHelpers
    {
        public static void IncreaseButton_Click(object sender, RoutedEventArgs e)
        {
            if (GetParentIntegerUpDown(sender) is IntegerUpDown control)
            {
                int current = control.Value ?? 0;
                int increment = control.Increment ?? 1;
                int max = control.Maximum ?? int.MaxValue;

                int newValue = current + increment;
                control.Value = newValue > max ? max : newValue;
            }
        }

        public static void DecreaseButton_Click(object sender, RoutedEventArgs e)
        {
            if (GetParentIntegerUpDown(sender) is IntegerUpDown control)
            {
                int current = control.Value ?? 0;
                int increment = control.Increment ?? 1;
                int min = control.Minimum ?? int.MinValue;

                int newValue = current - increment;
                control.Value = newValue < min ? min : newValue;
            }
        }

        private static IntegerUpDown GetParentIntegerUpDown(object sender)
        {
            DependencyObject obj = sender as DependencyObject;
            while (obj != null && obj is not IntegerUpDown)
            {
                obj = VisualTreeHelper.GetParent(obj);
            }
            return obj as IntegerUpDown;
        }
    }
}
