using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace Lab5
{
    public sealed partial class About : Page
    {
        public About()
        {
            this.InitializeComponent();
        }

        private void HyperlinkButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }
    }
}
