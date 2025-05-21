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
    /// Interaction logic for Statistics.xaml
    /// </summary>
    public partial class Statistics : Page
    {
        private Frame mainFrame;

        // Parameterless constructor required for XAML instantiation
        public Statistics()
        {
            InitializeComponent();
        }

        // Optional constructor with Frame parameter
        public Statistics(Frame frame) : this()
        {
            mainFrame = frame;
        }
    }

}
